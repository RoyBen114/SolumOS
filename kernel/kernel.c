unsigned char *videobuf = (unsigned char *)0xb8000;
void print(const char *str) {
    for (int i = 0; str[i]; i++)
    {
        videobuf[i * 2 + 0] = str[i];
        videobuf[i * 2 + 1] = 0x0F;
    }
}
void cls(){
    for (int i = 0; i < 80 * 25; i++)
    {
        videobuf[i * 2 + 0] = ' ';
        videobuf[i * 2 + 1] = 0x0F;
    }
}
int kernel_main()
{
    print("Welcome to Solum OS");
    return 0;
}