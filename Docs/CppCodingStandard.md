# Coding Standard

~~~cpp
const int g_GlobalVar;

class MyClass_t
{
 public:
    MyClass_t();
    ~MyClass_t();

    float PublicVar;

 private:
    uint32 m_PrivateVar;
};

void DoSomethingImportant(const int Param)
{
    int LocalVar = 1;

    for (int Count = 0; Count < 10; Count++)
    {
        LocalVar += Param;
    }
}
~~~