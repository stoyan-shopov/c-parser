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

*/

#endif


static const char * declarations =
/*
struct tag { int x; } s;
 */
"id\" s\" aggregate{ >>int struct-declarator-list{ id\" x\" }struct-declarator-list-end }aggregate-end id\" tag\" >struct define-variables declaration-end"
;

struct CIdentifier;
struct CDataType;

struct CStackNode
{
	enum CSTACK_NODE_ENUM
	{
		UNSPECIFIED	=	0,
		IDENTIFIER,
		DATA_TYPE,
		AGGREGATE_BEGIN,
		STRUCT_DECLARATOR_LIST_BEGIN,
		CSTACK_TAG_COUNT,
	};
private:
	static const char * tagNames[CSTACK_TAG_COUNT];
	enum CSTACK_NODE_ENUM _tag = UNSPECIFIED;
public:
	virtual CIdentifier * asIdentifier(void) { return 0; }
	virtual CDataType * asDataType(void) { return 0; }
	enum CSTACK_NODE_ENUM tag(void) { return _tag; }
	const char * tagName(void) { return (_tag < CSTACK_TAG_COUNT) ? tagNames[_tag] : "invalid tag value"; }
	CStackNode(enum CSTACK_NODE_ENUM tag) { _tag = tag; }
};
const char * CStackNode::tagNames[CSTACK_TAG_COUNT] =
{
	[CStackNode::UNSPECIFIED]	=	"unspecified",
	[CStackNode::IDENTIFIER]	=	"identifier",
	[CStackNode::DATA_TYPE]		=	"data type",
	[CStackNode::AGGREGATE_BEGIN]	=	"aggregate begin",
	[CStackNode::STRUCT_DECLARATOR_LIST_BEGIN]	=	"struct declarator list begin",
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
}
currentDataType
;

static QVector<CDataType> dataTypes;
static QMap<QString, QSharedPointer<CDataType>> structTags;

static QStack<QSharedPointer<CStackNode>> parseStack;

class Util
{
public:
	static void panic(std::string message) { qDebug() << "fatal error: " << QString::fromStdString(message); * (int *) 0 = 0; }
	static QSharedPointer<CStackNode> top(void) { if (!parseStack.size()) panic("stack empty"); return parseStack.top(); }
	static QSharedPointer<CStackNode> pop(void) { if (!parseStack.size()) panic("stack empty"); return parseStack.pop(); }
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
	static void dump(const CStackNode & n){}
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
}
void do_declaration_end(void)
{
	auto t = Util::pop();
	if (!t->asDataType())
		Util::panic("top of stack not a data type");
	else
	{
		CDataType * x = t->asDataType();
	}
}
void do_to_struct(void)
{
	/* test cases:


struct tag;
struct tag s;
struct { int x; };
struct { int x; } s;
struct tag { int x; };
struct tag { int x; } s;
struct tag;
struct tag_ext { struct tag_inner { struct tag_innermost; }; } ********** s[][10][20][30];


id" tag" >struct declaration-end

struct tag s;
id" s" id" tag" >struct define-variables declaration-end

struct { int x; };
aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end >struct declaration-end

struct { int x; } s;
id" s" aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end >struct define-variables declaration-end

struct tag { int x; };
aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end id" tag" >struct declaration-end

struct tag { int x; } s;
id" s" aggregate{ >>int struct-declarator-list{ id" x" }struct-declarator-list-end }aggregate-end id" tag" >struct define-variables declaration-end

struct tag_ext { struct tag_inner { struct tag_innermost; }; } ********** s[][10][20][30];
>pointer >pointer >pointer >pointer >pointer >pointer >pointer >pointer >pointer >pointer >array{ -2 }array-end >array{ -2 }array-end >array{ -2 }array-end >array[] id" s" aggregate{ aggregate{ id" tag_innermost" >struct }aggregate-end id" tag_inner" >struct }aggregate-end id" tag_ext" >struct define-variables declaration-end

struct tag_innermost { struct x ****** y; };
aggregate{ id" x" >struct struct-declarator-list{ id" y" >pointer >pointer >pointer >pointer >pointer >pointer }struct-declarator-list-end }aggregate-end id" tag_innermost" >struct declaration-end
	  */
}

void do_aggregate_begin(void){ parseStack.push(QSharedPointer<CDataType>(new CDataType(CDataType::AGGREGATE_BEGIN)));}
void do_struct_declarator_list_begin(void){ parseStack.push(QSharedPointer<CStackNode>(new CStackNode(CStackNode::STRUCT_DECLARATOR_LIST_BEGIN))); }
void do_struct_declarator_list_end(void)
{
	Util::dump();
auto s = Util::locate(CStackNode::AGGREGATE_BEGIN)->operator ->()->asDataType();
auto l = Util::locate(CStackNode::STRUCT_DECLARATOR_LIST_BEGIN), i = l + 1;

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
	Util::dump();
	do_abort();
}

/* extern "C" */}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	sf_reset();

	sf_eval(".( start of parsing)cr");
	sf_eval(declarations);
	sf_eval(".( parsing done)cr .s cr");

	return a.exec();
}
