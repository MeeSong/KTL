#include <stdio.h>
#include <crtdbg.h>
#include <new>

#include <KTL.Macro.h>
#include <KTL.Containers.Tuple.h>
#include <KTL.Functional.Bind.h>
#include <KTL.Functional.Function.h>
#include <KTL.Memory.SharedPtr.h>
#include <KTL.Memory.WeakPtr.h>
#include <KTL.Memory.Allocator.h>
#include <KTL.Containers.List.h>
#include <KTL.Strings.h>
//#include <KTL.Strings.Formator.h>

#define LOG(aFormat, ...) do { printf("%s : " aFormat "\n", __FUNCTION__, __VA_ARGS__); } while (0)

#define UNITTEST$BEGIN()    int vStatus = 0; for(;;) {
#define UNITTEST$CALL(f)    vStatus = (f()); if(vStatus){ printf("%d : Call function failed!\n", (__LINE__)); break; } printf("\n")
#define UNITTEST$CHECK_MEMORY_LEAK() LOG("Check memory leaks ? %s", _CrtDumpMemoryLeaks() ? "true" : "false")
#define UNITTEST$END()      break; } UNITTEST$CHECK_MEMORY_LEAK(); return vStatus;

//////////////////////////////////////////////////////////////////////////

auto LambdaAllocate = [](ktl::usize aSize)-> void* { auto vPtr = new ktl::byte[aSize]; LOG("alloc %Id bytes -> %p", aSize, vPtr); return vPtr; };
auto LambdaDeallocate = [](void* aPtr) -> void { LOG("dealloc %p", aPtr); delete[] aPtr; };

/// Test tuple

int TestTuple()
{
    int vStatus = 0;

    auto vTuple1  = ktl::make_tuple(ktl::u32(123), ktl::wchar(L'T'), (char*)("Hello KTL!"));
    auto vTuple2(vTuple1);

    for (;;)
    {
        LOG("Tuple1(%d, %wc, %s)", ktl::get<0>(vTuple1), ktl::get<1>(vTuple1), ktl::get<2>(vTuple1));
        LOG("Tuple2(%d, %wc, %s)", ktl::get<0>(vTuple2), ktl::get<1>(vTuple2), ktl::get<2>(vTuple2));

        auto vTuple3(ktl::move(vTuple2));
        LOG("Tuple3(%d, %wc, %s)", ktl::get<0>(vTuple3), ktl::get<1>(vTuple3), ktl::get<2>(vTuple3));

        LOG("Tuple3 == Tuple1 : %s", (vTuple3 == vTuple1) ? "true" : "false");

        break;
    }

    return vStatus;
}

/// functor & function

class TestFunctor
{
public:
    auto operator()(const ktl::wchar *a) -> int
    {
        LOG("%ws", a);
        return 0;
    }
};

auto TestFunction(const ktl::wchar *a) -> void
{
    LOG("%ws", a);
}

class TestBindThis
{
public:
    auto Test(const ktl::wchar *a) -> int
    {
        LOG("%ws", a);
        return 0;
    }
};

/// Test bind

int TestBind()
{
    auto vTestLambda = [](int /*aPlaceholder*/, const ktl::wchar *a) -> decltype(a)
    {
        LOG("%ws", a);
        return a;
    };

    auto vBind1(ktl::bind<void>(vTestLambda, 1, ktl::_1));
    auto vBind2(ktl::bind(TestFunctor(), ktl::_1));
    auto vBind3(ktl::bind(TestFunction, ktl::_1));

    TestBindThis vBindThis;
    auto vBind4(ktl::bind(&TestBindThis::Test, &vBindThis, ktl::_1));

    vBind1((L"1 Test Bind Lambda"));
    vBind2((L"2 Test Bind Functor"));
    vBind3((L"3 Test Bind Function"));
    vBind4((L"4 TestBindThis"));

    const bool vIs = ktl::is_member_function_pointer<typename ktl::decay<decltype(&TestBindThis::Test)>::type>::value;
    const bool vIs2 = ktl::is_member_object_pointer<typename ktl::decay<decltype(&TestBindThis::Test)>::type>::value;
    
    return 0;
}

/// Test function

int TestFunctional()
{
    auto vTestLambda = [](int /*aPlaceholder*/, const ktl::wchar *a) -> decltype(a)
    {
        LOG("%ws", a);
        return a;
    };

    ktl::function<decltype(TestFunction)> vTemplate(nullptr);
    
    decltype(vTemplate) vFun1;
    vFun1.attach(TestFunction);

    decltype(vTemplate) vFun2;
    vFun2.attach(ktl::reference_wrapper<decltype(TestFunction)>(TestFunction));

    decltype(vTemplate) vFun3;
    vFun3.attach(vFun1);

    decltype(vTemplate) vFun4;
    vFun4.attach(ktl::move(vFun2));

    vFun1(L"1 Test Functional Function");
    vFun2(L"2 Test Functional Empty");
    vFun3(L"3 Test Functional functional &");
    vFun4(L"4 Test Functional functional &&");

    vFun1.attach(ktl::bind<void>(vTestLambda, 1, ktl::_1));
    vFun2.attach(TestFunctor());

    vFun1(L"5 Test Functional Lambda");
    vFun2(L"6 Test Functional Functor");
    
    return 0;
}

/// Test shared_ptr

int TestSharedPtr()
{
    int vStatus = 0;

    ktl::shared_ptr<ktl::tuple<int>> vTemplate(nullptr);

    auto vLambda = [](decltype(vTemplate) a) -> void 
    {
        LOG("use_count == %d", a.use_count());
    };

    for (;;)
    {
        decltype(vTemplate) vPtr1;
        vPtr1.attach(new ktl::tuple<int>(1));

        decltype(vTemplate) vPtr2;
        ktl::make_shared(vPtr2, 2);

        decltype(vTemplate) vPtr3;
        vPtr3 = vPtr1;

        decltype(vTemplate) vPtr4;
        vPtr4 = ktl::move(vPtr2);

        // LOG("1 Test SharedPtr Empty : %d", ktl::get<0>(*vPtr2)); // Crash
        LOG("2 Test SharedPtr vPtr3 : %d", ktl::get<0>(*vPtr3));
        LOG("3 Test SharedPtr vPtr4 : %d", ktl::get<0>(*vPtr4));
        vLambda(vPtr1);

        break;
    }

    return vStatus;
}

/// Test weak_ptr

int TestWeakPtr()
{
    ktl::weak_ptr<ktl::u32> vWeakPtr;

    auto vLambda = [&vWeakPtr]() -> void
    {
        printf("%s : use_count == %d : ", __FUNCTION__, vWeakPtr.use_count());

        if (auto vPtr = vWeakPtr.lock())
        {
            printf("%d \n", *vPtr);
        }
        else
        {
            printf("weak is expired \n");
        }
    };

    for (;;)
    {
        {
            ktl::shared_ptr<ktl::u32> vPtr;
            ktl::make_shared<ktl::u32>(vPtr, ktl::u32(42));

            vWeakPtr = vPtr;
            vLambda();
        }

        vLambda();

        break;
    }

    return 0;
}

/// Test allocator

int TestAllocator()
{
    for (;;)
    {
        ktl::allocator<ktl::tuple<ktl::u32>> vAlloc;
        auto vPtr = vAlloc.allocate(1);
        vAlloc.construct(vPtr, 1);
        auto vPtr2 = vAlloc.address(vPtr[0]);
        auto vIsDefault = vAlloc.is_default();
        vAlloc.destroy(vPtr);
        vAlloc.deallocate(vPtr, 1);

        ktl::allocator<ktl::tuple<ktl::u32>> vAlloc2;
        ktl::function<void*(ktl::usize aSize)> vLambdaAlloc;
        ktl::function<void(void* aPtr)> vLambdaDealloc;

        vLambdaAlloc.attach(LambdaAllocate);
        vLambdaDealloc.attach(LambdaDeallocate);

        vAlloc2.construction(vLambdaAlloc, vLambdaDealloc);

        vPtr = vAlloc2.allocate(1);
        vAlloc.construct(vPtr, 2);
        vPtr2 = vAlloc2.address(vPtr[0]);
        vIsDefault = vAlloc2.is_default();
        vAlloc.destroy(vPtr);
        vAlloc2.deallocate(vPtr, 1);

        ktl::allocator<ktl::u32> vAlloc3;
        vAlloc3.construction(vAlloc2);

        break;
    }

    return 0;
}

/// Test list

int TestList()
{
    int vStatus = 1;

    ktl::function<void*(ktl::usize aSize)> vLambdaAlloc;
    ktl::function<void(void* aPtr)> vLambdaDealloc;
    vLambdaAlloc.attach(LambdaAllocate);
    vLambdaDealloc.attach(LambdaDeallocate);

    for (;;)
    {
        ktl::list<ktl::u32> vList1;
        if (!vList1.construction(5, 1)) break;
        if (!vList1.assign(10, 5)) break;
        if (!vList1.resize(15, 6)) break;
        vList1.sort();

        ktl::list<ktl::u32> vList2;
        
        for (ktl::u32 i = 0; i < 20; ++i)
        {
            vList2.emplace_front(i);
        }
        vList2.sort();
        if (!vList1.merge(vList2)) break;
        vList1.unique();
        vList1.remove(6);
        vList1.reverse();

        for (auto vValue : vList1)
        {
            printf("%d ", vValue);
        }
        printf("\n");
        LOG("Test list complate!");

        vStatus = 0;
        break;
    }

    return vStatus;
}

/// Test algorithm

int TestAlgorithm()
{
    LOG("Initialize list: ");

    ktl::list<ktl::u32> vList;
    for (ktl::u32 i = 0; i < 20; ++i)
    {
        vList.emplace_front(i);
        printf("%d ", i);
    }
    printf("\n");

    auto vCBegin = ktl::cbegin(vList);
    auto vCEnd = ktl::cend(vList);

    auto vPair = ktl::minmax_element(vCBegin, vCEnd);
    auto vmin = *ktl::get<0>(vPair);
    auto vmax = *ktl::get<1>(vPair);

    LOG("min = %d, max = %d", vmin, vmax);

    return 0;
}

/// Test string basic

int TestStringBasic()
{
    int vStatus = 1;

    for (;;)
    {
        if (0 != ktl::string_compare("meesong", 7, "meesong", 7)) break;
        if (1 != ktl::string_compare("abcdefgh", 8, "abcdefg", 7)) break;
        if (1 != ktl::string_compare("abcdefgh", 8, "ABCDEFGH", 8)) break;
        if (-1 != ktl::string_compare("ABCDEFGH", 8, "abcdefgh", 8)) break;
        if (-1 != ktl::string_compare("abcdefg", 7, "abcdefgh", 8)) break;

        if (0 != ktl::string_compare(L"meesong", 7, L"meesong", 7)) break;
        if (1 != ktl::string_compare(L"abcdefgh", 8, L"abcdefg", 7)) break;
        if (1 != ktl::string_compare(L"abcdefgh", 8, L"ABCDEFGH", 8)) break;
        if (-1 != ktl::string_compare(L"ABCDEFGH", 8, L"abcdefgh", 8)) break;
        if (-1 != ktl::string_compare(L"abcdefg", 7, L"abcdefgh", 8)) break;

        if (0 != ktl::string_compare_case_insensitive(L"ABCDEFGH", 8, L"abcdefgh", 8)) break;

        if (false == ktl::string_same("meesong", 7, "meesong", 7)) break;
        if (true == ktl::string_same("meesong", 7, "meesongs", 8)) break;

        if (nullptr == ktl::string_find_case_insensitive("meesong", 7, "SONG", 4)) break;
        if (nullptr == ktl::string_reverse_find_case_insensitive("meesong", 7, "MEE", 3)) break;

        ktl::isize vStrlen = 0;
        if (7 != (vStrlen = ktl::string_length("meesong"))) break;
        if (2 != (vStrlen = ktl::string_length(L"ÄãºÃ"))) break;

        char vStr1[16] = { "Hello MeeSong1" };
        ktl::string_copy(vStr1, KTL$CompileTime$ArraySize$Macro(vStr1), "Hello World2");
        LOG("Test string 1 : %s", vStr1);

        ktl::wchar vStr2[16] = { L"Hello MeeSong!" };
        ktl::string_append(vStr2, KTL$CompileTime$ArraySize$Macro(vStr2), L"Hello World!");
        LOG("Test string 2 : %ws", vStr2);

        char vStr3[16] = { 0 };
        ktl::memory_set(vStr3, KTL$CompileTime$ArraySize$Macro(vStr3), ktl::byte(-1));


        vStatus = 0;
        break;
    }

    return vStatus;
}

/// Test formator

//int TestFormator()
//{
//    int vStatus = 1;
//
//    for (;;)
//    {
//        int vInteger = 0;
//
//        ktl::wchar vFormatW[32]{};
//        ktl::Formator<ktl::wchar> vFormatorW;
//        vFormatorW.format(
//            vFormatW, KTL$CompileTime$ArraySize$Macro(vFormatW),
//            L"Test : ", ktl::usize(1234), L',', ktl::isize(-1), ',', ktl::byte(0), &vInteger);
//
//        LOG("format(%ws)", vFormatW);
//
//        char vFormatA[32]{};
//        ktl::Formator<char> vFormatorA;
//        vFormatorA.format(
//            vFormatA, KTL$CompileTime$ArraySize$Macro(vFormatA),
//            "Test : ", ktl::usize(1234), L',', ktl::isize(-1), ',', ktl::byte(0), &vInteger);
//
//        LOG("format(%s)", vFormatA);
//        
//
//        vStatus = 0;
//        break;
//    }
//
//    return vStatus;
//}

/// main

int main(int , char *[])
{
    UNITTEST$BEGIN()

        UNITTEST$CALL(TestTuple);
        UNITTEST$CALL(TestBind);
        UNITTEST$CALL(TestFunctional);
        UNITTEST$CALL(TestSharedPtr);
        UNITTEST$CALL(TestWeakPtr);
        UNITTEST$CALL(TestAllocator);
        UNITTEST$CALL(TestList);
        UNITTEST$CALL(TestStringBasic);
        //UNITTEST$CALL(TestFormator);

        getchar();

    UNITTEST$END()
}