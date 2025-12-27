#include <QTest>
#include "testinstructions.h"
#include "testassembler.h"
#include "testdisassembler.h"
#include "testmemory.h"
#include "testexecutionengine.h"

int main(int argc, char *argv[])
{
    int status = 0;
    {
        TestInstructions t1;
        status |= QTest::qExec(&t1, argc, argv);
    }
    {
        TestAssembler t2;
        status |= QTest::qExec(&t2, argc, argv);
    }
    {
        TestDisassembler t3;
        status |= QTest::qExec(&t3, argc, argv);
    }
    {
        TestMemory t4;
        status |= QTest::qExec(&t4, argc, argv);
    }
    {
        TestExecutionEngine t4;
        status |= QTest::qExec(&t4, argc, argv);
    }
    return status;
}
