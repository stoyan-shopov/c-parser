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

#if 1
/* struct str { struct { int x; }; struct {struct {int y;};};}; */
"declaration detected: aggregate{ aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end >struct >anonymous-aggregate aggregate{ aggregate{ >>int struct-declarator-list{ id\" y\" }struct-declarator-list-end }aggregate-end >struct >anonymous-aggregate }aggregate-end >struct >anonymous-aggregate }aggregate-end id\" str\" >struct declaration-end"
" "
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
		};
		unsigned typeSpecifiers;
	};
	CDataType(void) : CStackNode(DATA_TYPE) { typeSpecifiers = 0; }
	CDataType(enum CSTACK_NODE_ENUM tag) : CStackNode(tag) { typeSpecifiers = 0; }
	/* data details for 'struct' and 'union' aggregate types */
	QVector<QSharedPointer<CStackNode>>	members;
	QString					name;
};

struct Variable
{
	QVector<CStackNode> typeModifiers;
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
	static QVector<QSharedPointer<CStackNode>>::iterator  locate(enum CStackNode::CSTACK_NODE_ENUM tag)
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
		if (auto d = n.asDataType())
		{
			if (d->members.size())
			{
				qDebug() << "struct/union" << d->name << "{";
				for (auto i : d->members)
					dump(* i, indentation + 1);
				qDebug() << "}";
			}
			else
				qDebug() << "data type:" << d->name;
		}
		else if (auto id = n.asIdentifier())
		{
			qDebug() << "id:" << id->name;
		}
	}
	static void dumpTypes(void) { qDebug() << "data types\n"; for (auto & t : structTags) dump(t.operator *()); }
	static void dumpVariables(void) { qDebug() << "file scope variables\n"; for (auto & v : fileScopeVariables) qDebug() << v.name; }
};

extern "C"
{
#include "engine.h"
#include "sfext.h"

void do_id_quote(void) { sf_push('"'); do_parse(); int len = sf_pop(); parseStack.push(QSharedPointer<CIdentifier>(new CIdentifier((QByteArray((const char *) sf_pop(), len))))); }
void do_to_long(void) { if (!parseStack.size() || parseStack.top()->tag() != CStackNode::DATA_TYPE) parseStack.push(QSharedPointer<CDataType>(new CDataType)); parseStack.top()->asDataType()->isLong = true; }
void do_to_int(void) { if (!parseStack.size() || parseStack.top()->tag() != CStackNode::DATA_TYPE) parseStack.push(QSharedPointer<CDataType>(new CDataType)); parseStack.top()->asDataType()->isInt = true; }
void do_define_variables(void)
{
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
					Util::panic(QString("struct/union '") + id->name + "' not found");
				else
					parseStack.push(structTags[id->name]);
			}
		}
		else
			qDebug() << "struct declaration detected, no effect";
	}
	else if (t.tag() == CStackNode::AGGREGATE)
	{
		if (parseStack.size() == 1)
		{
			Util::drop();
			qDebug() << "warning: unnamed struct/union that defines no instances";
		}
		Util::dump();
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
	else if (s.asDataType())
	{
		/* handle nested struct/union declarations */
		Util::dump();
		auto s = Util::pop();
		if (Util::top().operator *().tag() != CStackNode::AGGREGATE_BEGIN)
			Util::panic("bad stack");
		auto t = Util::top().operator *().asDataType();
		t->members.append(s);
		t->setTag(CStackNode::AGGREGATE);
	}
	else
		Util::panic();
}

void do_struct_declarator_list_begin(void){ parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::STRUCT_DECLARATOR_LIST_BEGIN))); }
void do_struct_declarator_list_end(void)
{
auto s = Util::locate(CStackNode::AGGREGATE_BEGIN)->operator ->()->asDataType();
auto l = Util::locate(CStackNode::STRUCT_DECLARATOR_LIST_BEGIN), i = l + 1;

	Util::dump();
	while (i != parseStack.end())
	{
		if (i->operator *().asIdentifier())
			s->members.append(* i);
		++ i;
	}
	/* purge stack */
	int x = l - parseStack.begin();
	while (x != parseStack.size())
		parseStack.pop();
	if (Util::top()->asDataType())
		parseStack.pop();
}

void do_to_pointer(void)
{
	Util::dump();
	parseStack.push(QSharedPointer<CStackNode>(new CPointerModifier()));
}
void do_array_end(void)
{
	Util::dump();
	parseStack.push(QSharedPointer<CStackNode>(new CArrayModifier(sf_pop())));
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
