#ifndef CUSTOMTESTFRAMEWORK_H
#define CUSTOMTESTFRAMEWORK_H

using strLit = const char *;

#define TEST_ARGS(description) strLit args = "", strLit templateArgs = "", strLit desc = description

#define CONSUME_UNUSED_3(a, b, c) (void)a; (void)b; (void)c;
#define CONSUME_TEST_ARGS CONSUME_UNUSED_3(args, templateArgs, desc)

#define CALL_TESTX(testName, args, templateArgs) testName < PACK templateArgs > (PACK args, #args, #templateArgs)

#define STRINGIFY_SINGLE(x) #x
#define STRINGIFY(...) STRINGIFY_SINGLE((__VA_ARGS__))
#define CALL_TEST(testName, args, ...) testName __VA_ARGS__ (PACK args, #args, STRINGIFY(__VA_ARGS__))
#define CALL_TEST_DESC(testName, args, templateArgs, description) testName <PACK templateArgs> (PACK args, #args, #templateArgs, description)

#define PACK(...) __VA_ARGS__


template<typename T1>
void MySimpleTest(int a, TEST_ARGS("This is a description"))
{
      CONSUME_TEST_ARGS;
}


template<typename T1, typename T2, typename T3>
void MyTest(int a, int b, TEST_ARGS("This is a description"))
{
      CONSUME_TEST_ARGS;
}

void MyReqularTest1(int a, int b, TEST_ARGS("[MyReqularTest::something] : This is a very very long long description"))
{
      (void)a;
      (void)b;
      CONSUME_TEST_ARGS;
}

void MyReqularTest2(int a, int b, TEST_ARGS("[MyReqularTest::something] : This is a very very long long description"))
{
      (void)a;
      (void)b;
      CONSUME_TEST_ARGS;
}

void MyReqularTest3(int a, int b, TEST_ARGS("[MyReqularTest::something] : This is a very very long long description"))
{
      (void)a;
      (void)b;
      CONSUME_TEST_ARGS;
}

void MyReqularTest4(int a, int b, TEST_ARGS("[MyReqularTest::something] : This is a very very long long description"))
{
      (void)a;
      (void)b;
      CONSUME_TEST_ARGS;
}

void MyReqularTest5(int a, int b, TEST_ARGS("[MyReqularTest::something] : This is a very very long long description"))
{
      (void)a;
      (void)b;
      CONSUME_TEST_ARGS;
}

#define SOME_DEFINE1_X(a, b) a
#define SOME_DEFINE2_X(a, b) b

#define SOME_DEFINE1(pack1, pack2) SOME_DEFINE1_X pack1 + SOME_DEFINE1_X pack2
#define SOME_DEFINE2(pack1, pack2) SOME_DEFINE2_X pack

void TestInvoker(int a, int b)
{
    const char * str = STRINGIFY(some text , with comas);

    constexpr int _a = SOME_DEFINE1((1, 2), (8, 9));
    static_assert (_a == 9, "");

    CALL_TEST(MyTest, (10, 30), <int, double, int>);
    CALL_TEST_DESC(MyTest, (10, 30), (int, double, int), "Hello");
    CALL_TEST(MyTest, (10, 30), <int, double, int>);
    //CALL_TEST(MyTest, (15, 20), <char, double, float>);
    //CALL_TEST(MyReqularTest1, (10, 30));
}

#define COMPOSE_TEST_DECL(functionDeclaration, functionArguments, description) functionDeclaration ( PACK functionArguments , TEST_ARGS(description) )

#define SOME_REGISTRATION(functionDeclaration)

#define TEST_CASE(functionName, functionArguments, description) \
    COMPOSE_TEST_DECL(functionDeclaration, functionArguments, description); \
    \
    SOME_REGISTRATION(functionDeclaration) \
    \
    COMPOSE_TEST_DECL(functionDeclaration, functionArguments, description)

/*

  template<typename T>
  void MyTest(int a, int b, TEST_ARGS("This is a description"))
  {

  }

  TEST_CASE(void MyTest(int a, int b), "This is a description")
  {

  }

  |
  |
  V

  void MyTest(int a, int b);

  LocalTestManager.Register(MyTest, "This is a description");

  void MyTest(int a, int b)
  {

  }

  TEST_CASE(template<typename T>
            void MyTest, (int a, int b), "This is a description")
  {

  }


*/

void someTestFucn()
{

}

#endif // CUSTOMTESTFRAMEWORK_H
