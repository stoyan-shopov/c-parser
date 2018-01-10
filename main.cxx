#include <QString>
#include <QStack>
#include <QDebug>
#include <stack>
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

class Util
{
public:
	static void panic(std::string message) { qDebug() << "fatal error: " << QString::fromStdString(message); * (int *) 0 = 0; }
};

static const char * declarations = "id\" x\" >>int define-variables";

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
	/* data details for aggregate types (struct, unions, arrays) */
	QVector<CDataType>	members;
}
currentDataType
;

static QVector<CDataType> dataTypes;

static QStack<QSharedPointer<CStackNode>> parseStack;

extern "C"
{
#include "engine.h"
#include "sfext.h"

void do_id_quote(void) { sf_push('"'); do_parse(); int len = sf_pop(); parseStack.push(QSharedPointer<CIdentifier>(new CIdentifier((QByteArray((const char *) sf_pop(), len))))); }
void do_to_long(void) { if (!parseStack.size() || !parseStack.top()->asDataType()) parseStack.push(QSharedPointer<CDataType>(new CDataType)); parseStack.top()->asDataType()->isLong = true; }
void do_to_int(void) { if (!parseStack.size() || !parseStack.top()->asDataType()) parseStack.push(QSharedPointer<CDataType>(new CDataType)); parseStack.top()->asDataType()->isInt = true; }
void do_define_variables(void)
{
	if (parseStack.isEmpty())
		Util::panic("stack empty");
	auto t = parseStack.pop();
	if (!t->asDataType())
		Util::panic("top of stack not a data type");
}

}

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	sf_reset();

	sf_eval(".( start of parsing)cr");
	sf_eval(declarations);
	sf_eval(".( parsing done)cr .s cr");

	return a.exec();
}
