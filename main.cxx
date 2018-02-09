#include <QString>
#include <QStack>
#include <QDebug>
#include <QMap>
#include <QCoreApplication>
#include <QSharedPointer>

#include <string>

#include "sfext.h"

#define ENABLE_TEST_CASES	0


static const char * declarations =
"( file-level declaration detected:) declaration-with-declarator-list{ id\" x\" >>int define-variables }declaration-with-declarator-list"
" "
"( function definition detected:) compound-statement{ block-item{ expression{ lval{ id\" x\" } postfix++ }expression }block-item }compound-statement id\" main\" >function-param-type-list{ >>void >parameter-declaration-specifiers >parameter-list }function-param-type-list-end >>void define-funcion"
;

struct
{
	/* this is the original dictionary, after performing an sf_reset */
	word	* clean;
	word	* base;
	word	* expressions;
	static void switch_to(word * dictionary) { sf_push((cell) dictionary), sf_eval("latest !"); }
	static word * latest(void) { sf_eval("latest @"); return (word *) sf_pop(); }
} dictionaries;

struct CIdentifier;
struct CDataType;
struct CPointerModifier;
struct CArrayModifier;
struct CDataObject;
struct CStatement;

struct CStackNode
{
	enum CSTACK_NODE_ENUM
	{
		UNSPECIFIED	=	0,
		IDENTIFIER,
		DATA_TYPE,
		AGGREGATE_BEGIN,
		AGGREGATE,
		POINTER_MODIFIER,
		ARRAY_MODIFIER,
		FUNCTION_PARAMETER_TYPE_LIST_BEGIN,
		FUNCTION_PARAMETER_TYPE_LIST,
		FUNCTION_PARAMETER_ID_LIST_BEGIN,
		FUNCTION,
		DATA_OBJECT,
		DECLARATION_WITHOUT_DECLARATOR_LIST,
		DECLARATION_WITH_DECLARATOR_LIST,
		STATEMENT,
		COMPOUND_STATEMENT,
		EXPRESSION_STATEMENT,
		CSTACK_TAG_COUNT,
	};
private:
	static const char * tagNames[CSTACK_TAG_COUNT];
	enum CSTACK_NODE_ENUM _tag = UNSPECIFIED;
public:
	virtual CIdentifier * asIdentifier(void) { return 0; }
	virtual CDataType * asDataType(void) { return 0; }
	virtual struct CPointerModifier * asPointer(void) { return 0; }
	virtual struct CArrayModifier * asArray(void) { return 0; }
	virtual struct CDataObject * asDataobject(void) { return 0; }
	virtual struct CStatement * asStatement(void) { return 0; }

	void setTag(enum CSTACK_NODE_ENUM tag) { _tag = tag; }
	enum CSTACK_NODE_ENUM tag(void) { return _tag; }
	const char * tagName(void) { return (_tag < CSTACK_TAG_COUNT) ? tagNames[_tag] : "invalid tag value"; }
	CStackNode(enum CSTACK_NODE_ENUM tag) { _tag = tag; }
	CStackNode(void) { _tag = UNSPECIFIED; }
	virtual ~CStackNode(){}
};
const char * CStackNode::tagNames[CSTACK_TAG_COUNT] =
{
	[CStackNode::UNSPECIFIED]	=	"unspecified",
	[CStackNode::IDENTIFIER]	=	"identifier",
	[CStackNode::DATA_TYPE]		=	"data type",
	[CStackNode::AGGREGATE_BEGIN]	=	"aggregate begin",
	[CStackNode::AGGREGATE]		=	"aggregate",
	[CStackNode::POINTER_MODIFIER]	=	"pointer*",
	[CStackNode::ARRAY_MODIFIER]	=	"array[]",
	[CStackNode::FUNCTION_PARAMETER_TYPE_LIST_BEGIN]	=	"function parameter type list start",
	[CStackNode::FUNCTION_PARAMETER_TYPE_LIST]		=	"function parameter type list",
	[CStackNode::FUNCTION_PARAMETER_ID_LIST_BEGIN]		=	"function parameter id list start",
	[CStackNode::FUNCTION]		=				"function",
	[CStackNode::DATA_OBJECT]	=				"data object",
	[CStackNode::DECLARATION_WITHOUT_DECLARATOR_LIST]	=	"declaration without declarator list",
	[CStackNode::DECLARATION_WITH_DECLARATOR_LIST]	=		"declaration with declarator list",
	[CStackNode::STATEMENT]	=					"statement",
	[CStackNode::COMPOUND_STATEMENT]	=			"compound statement",
	[CStackNode::EXPRESSION_STATEMENT]	=			"expression statement",
};

struct CPointerModifier : public CStackNode
{
	CPointerModifier(void) : CStackNode(POINTER_MODIFIER) {}
	virtual struct CPointerModifier * asPointer(void) { return this; }
};
struct CArrayModifier : public CStackNode
{
	unsigned dimensionSize = 0;
	virtual struct CArrayModifier * asArray(void) { return this; }
	CArrayModifier(void) : CStackNode(ARRAY_MODIFIER) {}
	CArrayModifier(unsigned dimensionSize) : CStackNode(ARRAY_MODIFIER) { this->dimensionSize = dimensionSize; }
};

struct CIdentifier : CStackNode
{
	virtual CIdentifier * asIdentifier(void) { return this; }
	QString	name;
	CIdentifier(const QString & name) : CStackNode(IDENTIFIER) { this->name = name; }
	CIdentifier(const QString & name, enum CSTACK_NODE_ENUM tag) : CStackNode(tag) { this->name = name; }
};

struct CDataObject : public CStackNode
{
	QString					name;
	QSharedPointer<CStackNode>		type;
	CDataObject(void) : CStackNode(DATA_OBJECT) {}
	CDataObject(QSharedPointer<CStackNode> type) : CStackNode(DATA_OBJECT) { this->type = type; }
	CDataObject(const QString & name, QSharedPointer<CStackNode> type) : CStackNode(DATA_OBJECT) { this->type = type; this->name = name; }

	struct CDataObject * asDataobject(void) { return this; }
};

struct CDataType : CStackNode
{
	virtual CDataType * asDataType(void) { return this; }
	/* type specifier details */
	union
	{
		struct
		{
			bool	isVoid		: 1;
			bool	isInt		: 1;
			bool	isLong		: 1;
			bool	isStruct	: 1;
			bool	isArray		: 1;
			struct
			{
				/*! \todo	these field are messy, they are *NOT* mutually exclusive; maybe rework their handling */
				bool	isFunction			: 1;
				bool	isFunctionParameterList		: 1;
				bool	isFunctionParameterIdList	: 1;
			};
		};
		unsigned typeSpecifiers;
	};
	CDataType(void) : CStackNode(DATA_TYPE) { typeSpecifiers = 0; }
	CDataType(enum CSTACK_NODE_ENUM tag) : CStackNode(tag) { typeSpecifiers = 0; }
	QSharedPointer<CStackNode>		functionReturnType;
	QVector<CDataObject>			functionParameters;
	/* data details for 'struct' and 'union' aggregate types */
	QVector<CDataObject>			members;
	QString					name;
	QVector<QSharedPointer<CStackNode>>	typeModifiers;
};

struct CStatement : CStackNode
{
	virtual CStatement * asStatement(void) { return this; }

	CStatement(void) : CStackNode(DATA_TYPE) { }
	CStatement(enum CSTACK_NODE_ENUM tag) : CStackNode(tag) { }
	/* compound statement nested statements */
	QVector<CStatement>	statements;
};

static QVector<QSharedPointer<CStackNode>> dataTypes;
static QMap<QString, QSharedPointer<CStackNode>> structTags;
static QMap<QString, CDataObject> fileScopeVariables;

static QStack<QSharedPointer<CStackNode>> parseStack;

class Util
{
public:
	static void panic(QString message = "fatal error") { dump(); qDebug() << "fatal error: " << message; * (int *) 0 = 0; }
	static QSharedPointer<CStackNode> top(void) { if (!parseStack.size()) panic("stack empty"); return parseStack.top(); }
	static QSharedPointer<CStackNode> pop(void) { if (!parseStack.size()) panic("stack empty"); return parseStack.pop(); }
	static void drop(void) { pop(); }
	static QVector<QSharedPointer<CStackNode>>::iterator lastTypeNode(void)
	{
		if (parseStack.isEmpty())
			panic("stack empty");
		QVector<QSharedPointer<CStackNode>>::iterator i = parseStack.end() - 1;
		while (1)
		{
			if (i->operator *().asDataType())
				return i;
			if (i == parseStack.begin())
				break;
			-- i;
		}
		panic("no type node in stack");
		return parseStack.end();
		
	}

	static QVector<QSharedPointer<CStackNode>>::iterator locate(enum CStackNode::CSTACK_NODE_ENUM tag)
	{
		/*
		int i; for (i = parseStack.size() - 1; i >= 0; i --)
			if (parseStack[i]->tag() == tag)
				return parseStack.end();
				*/
		if (parseStack.isEmpty())
			panic("tag not found in stack");
		QVector<QSharedPointer<CStackNode>>::iterator i = parseStack.end() - 1;
		while (1)
		{
			if (i->operator *().tag() == tag)
				return i;
			if (i == parseStack.begin())
				break;
			-- i;
		}
		panic("tag not found in stack");
		return parseStack.end();
	}
	static void dump(void)
	{
		qDebug() << "----------------------";
		if (parseStack.isEmpty())
			qDebug() << "parse stack empty";
		else
		{
			qDebug() << "parse stack:" << parseStack.size() << "elements\n\n";
			for (auto i : parseStack)
				qDebug() << i->tagName();
		}
		qDebug() << "----------------------";
	}
	static void dump(CStackNode & n, int indentation = 0)
	{
		QString indent = QByteArray(indentation, '\t').constData();
		if (auto d = n.asDataType())
		{
			if (d->members.size())
			{
				qDebug().noquote() << indent << "struct/union" << d->name << "{";
				for (auto i : d->members)
				{
					QString modifiers;
					if (!i.type.isNull()) for (auto m : i.type->asDataType()->typeModifiers)
					{
						if (auto t = m->asArray())
							modifiers += "array[] ";
						else if (auto t = m->asPointer())
							modifiers += "pointer* ";
					}
					if (modifiers.size())
					qDebug().noquote() << indent << modifiers;
					dump(i.type.operator *(), indentation + 1);
					qDebug().noquote() << indent << i.name;
				}
				qDebug().noquote() << indent << "}";
			}
			else
				qDebug().noquote() << indent << "data type:" << d->name;
		}
		else if (auto id = n.asIdentifier())
			qDebug().noquote() << indent << "id:" << id->name;
		else
			panic();
	}
	static void dumpTypes(void) { qDebug() << "data types\n"; for (auto & t : structTags) dump(t.operator *()); }
	static void dumpVariables(void) { qDebug() << "file scope variables\n"; for (auto & v : fileScopeVariables) qDebug() << v.name; }
	static void fold_data_type(void)
	{
		/* inspects the stack, if the top of the stack contains a data type, and optionally any modifiers (e.g., pointers, arrays)
		 * below it, assembles them in a cumulative data type that replaces those nodes on the stack */
		if (!top()->asDataType())
			panic();
		auto t = QSharedPointer<CStackNode>(new CDataType(* pop()->asDataType()));
		while (!parseStack.empty())
		{
			auto x = top();
			if (x->asArray() || x->asPointer())
				t->asDataType()->typeModifiers.push_front(pop());
			else if (x->tag() == CStackNode::FUNCTION_PARAMETER_TYPE_LIST)
			{
				x = pop();
				x->asDataType()->functionReturnType = t;
				x->asDataType()->setTag(CStackNode::FUNCTION);
				x->asDataType()->isFunction = true;
				t = x;
			}
			else
				break;
		}
		parseStack.push(t);
	}
	static QVector<CDataObject> reap_data_objects(void)
	{
		QVector<CDataObject> d;
		auto t = pop();
		if (!t->asDataType())
			panic();
		do
		{
			parseStack.push(t);
			dump();
			fold_data_type();
			dump();
			auto x = pop();
			if (!top()->asIdentifier())
				panic();
			d.push_front(CDataObject(pop()->asIdentifier()->name, x));
		}
		while (!parseStack.empty()
			&& top()->tag() != CStackNode::AGGREGATE_BEGIN
			&& top()->tag() != CStackNode::DECLARATION_WITHOUT_DECLARATOR_LIST
			&& top()->tag() != CStackNode::DECLARATION_WITH_DECLARATOR_LIST
				);
		return d;
	}
	static QString declaration_string(QString s, CStackNode & n)
	{
		if (n.asDataobject())
		{
			if (n.asDataobject()->type.isNull())
				return QString("void ") + n.asDataobject()->name;
			else
				return declaration_string(n.asDataobject()->name, * n.asDataobject()->type);
		}
		else if (n.asDataType())
		{
			QString st = s;
			auto d = n.asDataType();
			int i;
			for (i = 0; i < d->typeModifiers.size(); i ++)
			{
				if (d->typeModifiers[i]->asArray())
				{
					st += QString("[%1]").arg(d->typeModifiers[i]->asArray()->dimensionSize);
				}
				else if (d->typeModifiers[i]->asPointer())
				{
					st.prepend("*");
					if (i != d->typeModifiers.size() - 1 && d->typeModifiers[i + 1]->asArray())
						st = QString("(") + st + ")";
					else if (i == d->typeModifiers.size() - 1 && d->isFunction)
						st = QString("(") + st + ")";
				}
				else
					panic();
			}
			if (d->isFunction)
			{
				QString params = "(";
				for (auto & p : d->functionParameters)
					params += declaration_string(QString(), p) + ", ";
				/* remove last comma */
				params.remove(params.length() - 2, 2);
				params += ")";
				return declaration_string(st + params, * d->functionReturnType);
			}
			else if (d->isStruct)
			{
				QString r("struct ");
				if (d->name.size())
					r += d->name + " ";
				if (d->members.size())
				{
					r += "{ ";
					for (auto & m : d->members)
					{
						r += declaration_string(QString(), m) + "; ";
					}
					r += "} ";
				}
				return r + st;
			}
			else if (d->isInt)
				return QString("int ") + st;
			else if (d->isLong)
				return QString("long ") + st;
			else if (d->isVoid)
				return QString("void ") + st;
			else
				panic();
		}
		panic();
	}
};

extern "C"
{
#include "engine.h"
#include "sfext.h"

void do_id_quote(void) { sf_push('"'); do_parse(); int len = sf_pop(); parseStack.push(QSharedPointer<CIdentifier>(new CIdentifier((QByteArray((const char *) sf_pop(), len))))); }
void do_to_long(void) { if (!parseStack.size() || parseStack.top()->tag() != CStackNode::DATA_TYPE) parseStack.push(QSharedPointer<CDataType>(new CDataType)); auto t = parseStack.top()->asDataType(); t->isLong = true; t->name += "long "; }
void do_to_int(void) { if (!parseStack.size() || parseStack.top()->tag() != CStackNode::DATA_TYPE) parseStack.push(QSharedPointer<CDataType>(new CDataType)); auto t = parseStack.top()->asDataType(); t->isInt = true; t->name += "int "; }
void do_to_void(void) { if (!parseStack.size() || parseStack.top()->tag() != CStackNode::DATA_TYPE) parseStack.push(QSharedPointer<CDataType>(new CDataType)); auto t = parseStack.top()->asDataType(); t->isVoid = true; t->name += "void "; }
void do_define_variables(void)
{
	Util::dump();
	auto d = Util::reap_data_objects();
	for (auto x : d)
	{
		qDebug() << "declaration:\n" << Util::declaration_string(QString(), x);
		if (fileScopeVariables.contains(x.name))
			qDebug() << "ERROR: file scope variable '" + x.name + "' redefined";
		else
			fileScopeVariables[x.name] = x;
	}
}
void do_declaration_end(void)
{
	auto t = Util::pop();
	if (t->tag() == CStackNode::DECLARATION_WITH_DECLARATOR_LIST)
	{
		if (!parseStack.empty())
			Util::panic();
		return;
	}
	if (!t->asDataType())
		Util::panic("top of stack not a data type");
	t = Util::pop();
	if (t->tag() != CStackNode::DECLARATION_WITHOUT_DECLARATOR_LIST
			|| !parseStack.empty())
		Util::panic("bad declaration");
}

void do_declaration_without_declarator_list_begin(void) { parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::DECLARATION_WITHOUT_DECLARATOR_LIST))); }
void do_declaration_with_declarator_list_begin(void) { parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::DECLARATION_WITH_DECLARATOR_LIST))); }

void do_compound_statement_begin(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CStatement(CStackNode::COMPOUND_STATEMENT)));
}

void do_to_struct(void)
{
auto & t = Util::top().operator *();

	if (t.asIdentifier())
	{
		auto x = Util::pop();
		auto id = x.operator *().asIdentifier();
		if (!parseStack.isEmpty())
		{
			if (Util::top().operator *().tag() == CStackNode::AGGREGATE)
			{
				/* handle named struct/union definitions */
				auto s = Util::top().operator ->()->asDataType();
				s->isStruct = true;
				s->name = id->name;
				dataTypes.append(Util::top());
				if (structTags.contains(s->name))
					qDebug() << "ERROR: struct/union" << s->name << "redefined";
				else
					structTags[s->name] = Util::top();
			}
			else
			{
				/* handle struct/union reference to an already defined struct/union */
				if (!structTags.contains(id->name))
				{
					Util::dump();
					qDebug() << QString("warning: struct/union '") + id->name + "' not found; constructing incomplete type node";
					auto t = QSharedPointer<CStackNode>(new CDataType(CStackNode::AGGREGATE));
					auto s = t.operator*().asDataType();
					s->isStruct = true;
					s->name = id->name;
					parseStack.push(t);
				}
				else
					parseStack.push(structTags[id->name]);
			}
		}
		else
			qDebug() << "struct declaration detected, no effect";
	}
	else if (t.tag() == CStackNode::AGGREGATE)
	{
		t.asDataType()->isStruct = true;
		if (parseStack.size() == 1)
		{
			Util::drop();
			qDebug() << "warning: unnamed struct/union that defines no instances";
		}
	}
	else
		Util::panic();
}

void do_aggregate_begin(void){ parseStack.push(QSharedPointer<CDataType>(new CDataType(CDataType::AGGREGATE_BEGIN)));}
void do_aggregate_end(void)
{
auto & s = Util::top().operator *();

	if (s.tag() == CStackNode::AGGREGATE_BEGIN)
	{
		s.setTag(CStackNode::AGGREGATE);
	}
	else
		Util::panic();
}

void do_to_struct_declaration(void)
{
/*
aggregate begin
aggregate begin
identifier
identifier
pointer*
pointer*
pointer*
array[]
pointer*
array[]
identifier
pointer*
identifier
array[]
identifier
array[]
pointer*
identifier
data type
*/
	auto m = Util::reap_data_objects();
	auto s = Util::top();
	if (s->tag() != CStackNode::AGGREGATE_BEGIN)
		Util::panic();
	s->asDataType()->members << m;
}

void do_to_anonymous_aggregate(void)
{
/* anonymous struct/union member nested in another struct/union */
auto s = Util::pop(), c = Util::top();

	if (s->tag() != CStackNode::AGGREGATE || c->tag() != CStackNode::AGGREGATE_BEGIN)
		/*! \todo	this should be fixed, this is valid C code: 'struct { int; };' */
		Util::panic();
	c->asDataType()->members.append(CDataObject(s));
}

void do_to_pointer(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CPointerModifier()));
}
void do_array_end(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CArrayModifier(sf_pop())));
}

void do_to_function_parameter_type_list_begin(void)
{
	Util::dump();
	parseStack.push(QSharedPointer<CStackNode>(new CDataType(CStackNode::FUNCTION_PARAMETER_TYPE_LIST_BEGIN)));
}

void do_to_parameter_list(void)
{
	auto p = Util::pop();
	auto l = Util::top();
	if (l->tag() != CStackNode::FUNCTION_PARAMETER_TYPE_LIST_BEGIN)
		Util::panic();
	l->asDataType()->functionParameters << * p->asDataobject();
}

void do_to_id_list(void)
{
	auto id = Util::pop();
	auto l = Util::top();
	if (l->tag() != CStackNode::FUNCTION_PARAMETER_ID_LIST_BEGIN)
		Util::panic();
	CDataObject d;
	d.name = id->asIdentifier()->name;
	l->asDataType()->functionParameters << d;
}

void do_to_abstract_parameter_declaration(void)
{
	Util::fold_data_type();
	parseStack.push(QSharedPointer<CStackNode>(new CDataObject(Util::pop())));
}
void do_to_parameter_declaration(void)
{
	Util::fold_data_type();
	auto t = Util::pop();
	auto id = Util::pop();
	parseStack.push(QSharedPointer<CStackNode>(new CDataObject(id->asIdentifier()->name, t)));
}

void do_to_parameter_declaration_specifiers(void)
{
	Util::fold_data_type();
	parseStack.push(QSharedPointer<CStackNode>(new CDataObject(Util::pop())));
}

void do_function_parameter_type_list_end(void)
{
	auto l = Util::top();
	if (l->tag() != CStackNode::FUNCTION_PARAMETER_TYPE_LIST_BEGIN)
		Util::panic();
	l->setTag(CStackNode::FUNCTION_PARAMETER_TYPE_LIST);
	l->asDataType()->isFunctionParameterList = true;
}

void do_function_id_list_end(void)
{
	if (Util::top()->tag() != CStackNode::FUNCTION_PARAMETER_ID_LIST_BEGIN)
		Util::panic();
	Util::top()->setTag(CStackNode::FUNCTION_PARAMETER_TYPE_LIST_BEGIN);
	do_function_parameter_type_list_end();
	Util::top()->asDataType()->isFunctionParameterIdList = true;
}


void do_to_empty_array(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CArrayModifier(0)));
}

void do_to_abstract_declarator_array(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CArrayModifier(0)));
}

void do_to_function_id_list_begin(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CDataType(CStackNode::FUNCTION_PARAMETER_ID_LIST_BEGIN)));
}

void empty(void){}

/* extern "C" */}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	sf_reset();
	dictionaries.clean = dictionaries.latest();
	sf_expression_dictionary_init();
	dictionaries.expressions = dictionaries.latest();
	dictionaries.switch_to(dictionaries.clean);
	sf_base_dictionary_init();
	dictionaries.base = dictionaries.latest();

	sf_eval(".( start of parsing)cr");
	sf_eval(declarations);
	sf_eval(".( parsing done)cr .s cr");

	Util::dumpTypes();
	Util::dumpVariables();

	return a.exec();
}
