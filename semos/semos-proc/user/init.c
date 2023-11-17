extern void syscall(int,...);

int main()
{
    syscall(1);
    syscall(2,3,4);
    syscall(42);
    while(1);
    return 0;
}
