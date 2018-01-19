#include <QString>
#include <QStack>
#include <QDebug>
#include <QMap>
#include <QCoreApplication>
#include <QSharedPointer>

#include <string>

#if 0

/*

long long long
long long long
long long long;
>>long >>long >>long >>long >>long >>long >>long >>long >>long declaration-end

long int short signed struct x a;
id" a" id" x" >struct >>signed >>short >>int >>long define-variables declaration-end

struct str { int y; } x;
id" x" aggregate{ >>int struct-declarator-list{ id" y" }struct-declarator-list-end }aggregate-end id" str" >struct define-variables declaration-end

int x;
id" x" >>int define-variables declaration-end

int x, y, z;
id" x" id" y" id" z" >>int define-variables declaration-end

struct
{
	int x, y;
}
s;
id" s" aggregate{ >>int struct-declarator-list{ id" x" id" y" }struct-declarator-list-end }aggregate-end >struct define-variables declaration-end

struct str
{
	int x, y;

	struct { int a, b; struct { int xxx; }; } *** arr[][10][20][30];
}
id" s" aggregate{ >>int struct-declarator-list{ id" x" id" y" }struct-declarator-list-end aggregate{ >>int struct-declarator-list{ id" a" id" b" }struct-declarator-list-end aggregate{ >>int struct-declarator-list{ id" xxx" }struct-declarator-list-end }aggregate-end >struct }aggregate-end >struct struct-declarator-list{ >pointer >pointer >pointer >array{ -2 }array-end >array{ -2 }array-end >array{ -2 }array-end >array[] id" arr" }struct-declarator-list-end }aggregate-end id" str" >struct define-variables declaration-end


int ** x;
int ** y[];
int (** z)[];
int *(* a)[], b, c;


struct str
{
	struct s *(*(* p)[10][10])[12], z;
	struct { int x, y, z; } **(*x)[10], d;
};

struct str1
{
	struct s *(*(* p)[10][10])[12], z;
	struct { int x, y, z; } **(*x)[10], d;
} ***(**p[5])[10];


id" x" >pointer >pointer >>int define-variables declaration-end
id" y" >array[] >pointer >pointer >>int define-variables declaration-end
id" z" >pointer >pointer >array[] >>int define-variables declaration-end
id" a" >pointer >array[] >pointer id" b" id" c" >>int define-variables declaration-end

*/

#endif

#define ENABLE_TEST_CASES	0


static const char * declarations =
/*
struct s { struct {int x;};};

struct str
{
	struct s *(*(* p)[10][10])[12], z;
	struct { int x, y, z; } **(*x)[10], d;
};*/
#if 0
"aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end >struct }aggregate-end id\" s\" >struct declaration-end"
" "
"aggregate{ id\" s\" >struct struct-declarator-list{ id\" p\" >pointer >array{ -2 }array-end >array{ -2 }array-end >pointer >array{ -2 }array-end >pointer id\" z\" }struct-declarator-list-end aggregate{ >>int struct-declarator-list{ id\" x\" id\" y\" id\" z\" }struct-declarator-list-end }aggregate-end >struct struct-declarator-list{ id\" x\" >pointer >array{ -2 }array-end >pointer >pointer id\" d\" }struct-declarator-list-end }aggregate-end id\" str\" >struct declaration-end"
" "
#endif

#if 0
/* struct str { struct { int x; } s1; struct {struct {int y;};}s2;}; */
"aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end >struct struct-declarator-list{ id\" s1\" }struct-declarator-list-end aggregate{ aggregate{ >>int struct-declarator-list{ id\" y\" }struct-declarator-list-end }aggregate-end >struct }aggregate-end >struct struct-declarator-list{ id\" s2\" }struct-declarator-list-end }aggregate-end id\" str\" >struct declaration-end"
#endif

#if ENABLE_TEST_CASES
/* struct str { struct { int x; }; struct {struct {int y;};};}; */
"aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end >struct >anonymous-aggregate aggregate{ aggregate{ >>int struct-declarator-list{ id\" y\" }struct-declarator-list-end }aggregate-end >struct >anonymous-aggregate }aggregate-end >struct >anonymous-aggregate }aggregate-end id\" str\" >struct declaration-end"
" "
#endif


#if 1
/* int (*fpfi1(struct { int x; } *, struct x { int y; }*[], struct z { int y; }(*xxx)[]))(struct foo *); */
"id\" fpfi1\" >function-param-type-list{ id\" xxx\" >pointer >array[] aggregate{ >>int struct-declarator-list{ id\" y\" }struct-declarator-list-end }aggregate-end id\" z\" >struct |param-list-boundary| >abstract-declarator-array[] >pointer aggregate{ >>int struct-declarator-list{ id\" y\" }struct-declarator-list-end }aggregate-end id\" x\" >struct |param-list-boundary| >pointer aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end >struct }function-param-type-list-end >pointer >function-param-type-list{ >pointer id\" foo\" >struct }function-param-type-list-end >>int define-variables declaration-end"
#endif
#if 0
/* struct str { struct { int r, (*(***x)[10])[20], * a, b[10], *c[10], z; }; struct {struct {int y;};};}; */
"aggregate{ aggregate{ >>int struct-declarator-list{ id\" r\" id\" x\" >pointer >pointer >pointer >array{ -2 }array-end >pointer >array{ -2 }array-end id\" a\" >pointer id\" b\" >array{ -2 }array-end id\" c\" >array{ -2 }array-end >pointer id\" z\" }struct-declarator-list-end }aggregate-end >struct >anonymous-aggregate aggregate{ aggregate{ >>int struct-declarator-list{ id\" y\" }struct-declarator-list-end }aggregate-end >struct >anonymous-aggregate }aggregate-end >struct >anonymous-aggregate }aggregate-end id\" str\" >struct declaration-end"
" "		
#if 0
aggregate{
	aggregate{
		>>int struct-declarator-list{
id" r" id" x" >pointer >pointer >pointer >array{ -2 }array-end >pointer >array{ -2 }array-end id" a" >pointer id" b" >array{ -2 }array-end id" c" >array{ -2 }array-end >pointer id" z"
		}struct-declarator-list-end
	}aggregate-end >struct >anonymous-aggregate
	aggregate{
		aggregate{ >>int struct-declarator-list{ id" y" }struct-declarator-list-end
		}aggregate-end >struct >anonymous-aggregate
	}aggregate-end >struct >anonymous-aggregate
}aggregate-end id" str" >struct declaration-end
#endif
#endif

#if ENABLE_TEST_CASES
/* struct s1; */
"id\" s1\" >struct declaration-end"
" "
#endif
#if ENABLE_TEST_CASES
/* struct { struct s2 { int x; }; }; */
"aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end id\" s2\" >struct }aggregate-end >struct declaration-end"
" "
#endif
#if ENABLE_TEST_CASES
/* struct { struct s3 { int x; }; } str;*/
"id\" str\" aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end id\" s3\" >struct }aggregate-end >struct define-variables declaration-end"
" "
#endif
#if ENABLE_TEST_CASES
/* same as above - test duplicate detection */
/* struct { struct s3 { int x; }; } str; */
"id\" str\" aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end id\" s3\" >struct }aggregate-end >struct define-variables declaration-end"
" "
#endif
#if ENABLE_TEST_CASES
/* struct s1 { int x; }; */
"aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end id\" s1\" >struct declaration-end"
" "
#endif

#if ENABLE_TEST_CASES
/* struct s1 s; */
"id\" s\" id\" s1\" >struct define-variables declaration-end"
" "
#endif

#if ENABLE_TEST_CASES
/* struct { struct { int x; }; }; */
"aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end >struct }aggregate-end >struct declaration-end"
" "
#endif

#if ENABLE_TEST_CASES
/* struct { struct { int x; }; } s1; */
"id\" s1\" aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end >struct }aggregate-end >struct define-variables declaration-end"
" "
#endif

#if ENABLE_TEST_CASES
/* struct s4 { struct { int x; }; }; */
"aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end >struct }aggregate-end id\" s4\" >struct declaration-end"
" "
#endif

#if ENABLE_TEST_CASES
/* struct s5 { struct s6 { int x; }; } s2; */
"id\" s2\" aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end id\" s6\" >struct }aggregate-end id\" s5\" >struct define-variables declaration-end"
" "
#endif

;

struct CIdentifier;
struct CDataType;
struct CPointerModifier;
struct CArrayModifier;


struct CStackNode
{
	enum CSTACK_NODE_ENUM
	{
		UNSPECIFIED	=	0,
		IDENTIFIER,
		DATA_TYPE,
		AGGREGATE_BEGIN,
		AGGREGATE,
		STRUCT_DECLARATOR_LIST_BEGIN,
		POINTER_MODIFIER,
		ARRAY_MODIFIER,
		FUNCTION_PARAMETER_TYPE_LIST_BEGIN,
		FUNCTION_PARAMETER_TYPE_LIST_END,
		FUNCTION_PARAMETER_ID_LIST_BEGIN,
		FUNCTION_PARAMETER_ID_LIST_END,
		FUNCTION_PARAMETER_TYPE_LIST_BOUNDARY,
		ABSTRACT_DECLARATOR_FUNCTION_ID_LIST_BEGIN,
		ABSTRACT_DECLARATOR_FUNCTION_ID_LIST_END,
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
	[CStackNode::STRUCT_DECLARATOR_LIST_BEGIN]	=	"struct declarator list begin",
	[CStackNode::POINTER_MODIFIER]	=	"pointer*",
	[CStackNode::ARRAY_MODIFIER]	=	"array[]",
	[CStackNode::FUNCTION_PARAMETER_TYPE_LIST_BEGIN]	=	"function parameter type list start",
	[CStackNode::FUNCTION_PARAMETER_TYPE_LIST_END]		=	"function parameter type list end",
	[CStackNode::FUNCTION_PARAMETER_ID_LIST_BEGIN]		=	"function parameter id list start",
	[CStackNode::FUNCTION_PARAMETER_ID_LIST_END]		=	"function parameter id list end",
	[CStackNode::FUNCTION_PARAMETER_TYPE_LIST_BOUNDARY]	=	"function parameter type list boundary",
	[CStackNode::ABSTRACT_DECLARATOR_FUNCTION_ID_LIST_BEGIN]	=	"abstract declarator function id list start",
	[CStackNode::ABSTRACT_DECLARATOR_FUNCTION_ID_LIST_END]	=	"abstract declarator function id list end",
};

struct CPointerModifier : public CStackNode
{
	CPointerModifier(void) : CStackNode(POINTER_MODIFIER) {}
	virtual struct CPointerModifier * asPointer(void) { return this; }
};
struct CArrayModifier : public CStackNode
{
	unsigned dimensionSize;
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

struct DataObject
{
	QString					name;
	QSharedPointer<CStackNode>		type;
	QVector<QSharedPointer<CStackNode>>	typeModifiers;
	DataObject(void) {}
	DataObject(QSharedPointer<CStackNode> type) { this->type = type; }
};

struct CDataType : CStackNode
{
	virtual CDataType * asDataType(void) { return this; }
	/* type specifier details */
	union
	{
		struct
		{
			bool	isInt		: 1;
			bool	isLong		: 1;
			bool	isStruct	: 1;
			bool	isArray		: 1;
			bool	isFunction	: 1;
		};
		unsigned typeSpecifiers;
	};
	CDataType(void) : CStackNode(DATA_TYPE) { typeSpecifiers = 0; }
	CDataType(enum CSTACK_NODE_ENUM tag) : CStackNode(tag) { typeSpecifiers = 0; }
	QSharedPointer<CStackNode>		functionReturnType;
	QVector<DataObject>			functionArguments;
	/* data details for 'struct' and 'union' aggregate types */
	QVector<DataObject>			members;
	QString					name;
};

struct Variable
{
	QVector<QSharedPointer<CStackNode>> typeModifiers;
	QString name;
	QSharedPointer<CStackNode>	type;
	Variable(const QString & name, QSharedPointer<CStackNode> type) { this->name = name, this->type = type; }
	Variable(void){}
};

static QVector<QSharedPointer<CStackNode>> dataTypes;
static QMap<QString, QSharedPointer<CStackNode>> structTags;
static QMap<QString, Variable> fileScopeVariables;

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
					for (auto m : i.typeModifiers)
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
};

extern "C"
{
#include "engine.h"
#include "sfext.h"

void do_id_quote(void) { sf_push('"'); do_parse(); int len = sf_pop(); parseStack.push(QSharedPointer<CIdentifier>(new CIdentifier((QByteArray((const char *) sf_pop(), len))))); }
void do_to_long(void) { if (!parseStack.size() || parseStack.top()->tag() != CStackNode::DATA_TYPE) parseStack.push(QSharedPointer<CDataType>(new CDataType)); auto t = parseStack.top()->asDataType(); t->isLong = true; t->name += "long "; }
void do_to_int(void) { if (!parseStack.size() || parseStack.top()->tag() != CStackNode::DATA_TYPE) parseStack.push(QSharedPointer<CDataType>(new CDataType)); auto t = parseStack.top()->asDataType(); t->isInt = true; t->name += "int "; }
void do_define_variables(void)
{
	Util::dump();
	Util::panic();
	auto t = Util::pop();
	if (!t->asDataType())
		Util::panic("top of stack not a data type");
	do
	{
		auto d = Util::pop();
		auto id = d.operator *() .asIdentifier();
		if (!id)
			Util::panic();
		if (fileScopeVariables.contains(id->name))
			qDebug() << "ERROR: file scope variable '" + id->name + "' redefined";
		else
			fileScopeVariables[id->name] = Variable(id->name, t);
	}
	while (!parseStack.isEmpty());
}
void do_declaration_end(void)
{
	if (parseStack.isEmpty())
		return;
	auto t = Util::pop();
	if (!parseStack.isEmpty())
		Util::panic();
	if (!t->asDataType())
		Util::panic("top of stack not a data type");
}
void do_to_struct(void)
{
	/* test cases:
id" s1" >struct declaration-end
struct s1;

aggregate{ aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end id" s2" >struct }aggregate-end >struct declaration-end
struct { struct s2 { int x; }; };

id" str" aggregate{ aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end id" s3" >struct }aggregate-end >struct define-variables declaration-end
struct { struct s3 { int x; }; } str;

aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end id" s1" >struct declaration-end
struct s1 { int x; };

id" s" id" s1" >struct define-variables declaration-end
struct s1 s;

aggregate{ aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end >struct }aggregate-end >struct declaration-end
struct { struct { int x; }; };

id" s1" aggregate{ aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end >struct }aggregate-end >struct define-variables declaration-end
struct { struct { int x; }; } s1;

aggregate{ aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end >struct }aggregate-end id" s4" >struct declaration-end
struct s4 { struct { int x; }; };

id" s2" aggregate{ aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end id" s6" >struct }aggregate-end id" s5" >struct define-variables declaration-end
struct s5 { struct s6 { int x; }; } s2;
	  */

/*struct { struct s2 { int x; }; };*/
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

void do_struct_declarator_list_begin(void){ parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::STRUCT_DECLARATOR_LIST_BEGIN))); }
void do_struct_declarator_list_end(void)
{
DataObject d;
/*
aggregate begin
aggregate begin
data type
struct declarator list begin
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
----------------------
*/
{
	/* sanity checks */
auto s = Util::locate(CStackNode::AGGREGATE_BEGIN);
auto t = Util::lastTypeNode();
auto l = Util::locate(CStackNode::STRUCT_DECLARATOR_LIST_BEGIN);
	if (l - t != 1 || t - s != 1)
		Util::panic("bad stack");
	d.type = * t;
}
QVector<DataObject> members;
auto n = Util::pop();
	do
	{
		if (n.operator *() .asPointer() || n.operator *() .asArray())
			d.typeModifiers.push_front(n);
		else if (n.operator *().asIdentifier())
		{
			d.name = n.operator *().asIdentifier()->name;
			members.push_front(d);
			d.typeModifiers.clear();
		}
		else
			Util::panic("bad stack");
		n = Util::pop();
	}
	while (n.operator *().tag() != CStackNode::STRUCT_DECLARATOR_LIST_BEGIN);
	/* clean up stack */
	/* drop member data type */
	Util::drop();
	/* add member list just built to the containing struct/union */
	auto s = Util::top().operator *().asDataType();
	s->members += members;
}

void do_to_anonymous_aggregate(void)
{
/* anonymous struct/union member nested in another struct/union */
auto s = Util::pop(), c = Util::top();
	if (s->tag() != CStackNode::AGGREGATE || c->tag() != CStackNode::AGGREGATE_BEGIN)
		Util::panic();
	c->asDataType()->members.append(DataObject(s));
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
	parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::FUNCTION_PARAMETER_TYPE_LIST_BEGIN)));
}

void do_to_function_parameter_type_list_end(void)
{
	Util::dump();
	parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::FUNCTION_PARAMETER_TYPE_LIST_END)));
}

void do_to_empty_array(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CArrayModifier(0)));
}

void do_to_abstract_declarator_array(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CArrayModifier(0)));
}

void do_parameter_list_boundary(void)
{
	Util::dump();
	parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::FUNCTION_PARAMETER_TYPE_LIST_BOUNDARY)));
}

void do_to_abstract_declarator_function_id_list_begin(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::ABSTRACT_DECLARATOR_FUNCTION_ID_LIST_BEGIN)));
}

void do_abstract_declarator_function_id_list_end(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::ABSTRACT_DECLARATOR_FUNCTION_ID_LIST_END)));
}

void do_to_function_id_list_begin(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::ABSTRACT_DECLARATOR_FUNCTION_ID_LIST_BEGIN)));
}

void do_function_id_list_end(void)
{
	parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::ABSTRACT_DECLARATOR_FUNCTION_ID_LIST_END)));
}

void empty(void){}

/* extern "C" */}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	sf_reset();

	sf_eval(".( start of parsing)cr");
	sf_eval(declarations);
	sf_eval(".( parsing done)cr .s cr");

	Util::dumpTypes();
	Util::dumpVariables();

	return a.exec();
}
