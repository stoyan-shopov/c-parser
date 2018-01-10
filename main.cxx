#include <QString>
#include <QStack>
#include <QDebug>
#include <stack>
#include <QCoreApplication>

#include <string>

#if 0

/*

long int short signed struct x a;
id" a" id" x" >struct >>signed >>short >>int >>long define-variables

int x;
id" x" >>int define-variables

int x, y, z;
id" x" id" y" id" z" >>int define-variables

struct
{
	int x, y;
}
s;
id" s" aggregate{ >>int struct-declarator-list{ id" x" id" y" }struct-declarator-list-end }aggregate-end >struct define-variables

struct str
{
	int x, y;

	struct { int a, b; struct { int xxx; }; } *** arr[][10][20][30];
}
s;
id" s" aggregate{ >>int struct-declarator-list{ id" x" id" y" }struct-declarator-list-end aggregate{ >>int struct-declarator-list{ id" a" id" b" }struct-declarator-list-end aggregate{ >>int struct-declarator-list{ id" xxx" }struct-declarator-list-end }aggregate-end >struct }aggregate-end >struct struct-declarator-list{ >pointer >pointer >pointer >array{ -2 }array-end >array{ -2 }array-end >array{ -2 }array-end >array[] id" arr" }struct-declarator-list-end }aggregate-end id" str" >struct define-variables

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
	struct
	{
		int	isInt		: 1;
		int	isStruct	: 1;
		int	isArray		: 1;
	};
	CDataType(void) { isInt = isStruct = isArray = 0; }
	/* data details for aggregate types (struct, unions, arrays) */
	QVector<CDataType>	aggregate;
}
currentDataType
;

static QVector<CDataType> dataTypes;

static QStack<CStackNode> parseStack;

extern "C"
{
#include "engine.h"
#include "sfext.h"

void do_id_quote(void) { sf_push('"'); do_parse(); int len = sf_pop(); parseStack.push(CIdentifier(QByteArray((const char *) sf_pop(), len))); }
void do_to_int(void) {}
void do_define_variables(void)
{
	if (parseStack.isEmpty())
		Util::panic("stack empty");
	auto t = parseStack.pop();
	if (!t.asDataType())
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

	qDebug() << 1;

	return a.exec();
}
