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
long long long
long long long
long long long;
*/
">>long >>long >>long >>long >>long >>long >>long >>long >>long declaration-end"
;

struct CIdentifier;
struct CDataType;

struct CStackNode
{
	virtual CIdentifier * asIdentifier(void) { return 0; }
	virtual CDataType * asDataType(void) { return 0; }
};

struct CIdentifier : CStackNode
{
	virtual CIdentifier * asIdentifier(void) { return this; }
	QString	name;
	CIdentifier(const QString & name) { this->name = name; }
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
	CDataType(void) { typeSpecifiers = 0; }
	/* data details for 'struct' and 'union' aggregate types */
	QVector<CDataType>	members;
	bool isAggregate(void) { return members.size() != 0; }
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
};

extern "C"
{
#include "engine.h"
#include "sfext.h"

void do_id_quote(void) { sf_push('"'); do_parse(); int len = sf_pop(); parseStack.push(QSharedPointer<CIdentifier>(new CIdentifier((QByteArray((const char *) sf_pop(), len))))); }
void do_to_long(void) { if (!parseStack.size() || !parseStack.top()->asDataType()) parseStack.push(QSharedPointer<CDataType>(new CDataType)); parseStack.top()->asDataType()->isLong = true; }
void do_to_int(void) { if (!parseStack.size() || !parseStack.top()->asDataType()) parseStack.push(QSharedPointer<CDataType>(new CDataType)); parseStack.top()->asDataType()->isInt = true; }
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
	  */
	auto t = Util::pop();
	if (t->asIdentifier())
	{
		auto name = t->asIdentifier()->name;
		/* named structure */
		if (structTags.contains(name))
			qDebug() << "warning: duplicate structure tag found in program";
		t = Util::pop();
		if (!t->asDataType())
		structTags[name] = Util::po
	}
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
