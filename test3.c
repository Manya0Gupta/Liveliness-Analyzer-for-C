int main() {
    int a;
    int b;
    a = 10;
    if (a > 5) {
        a = 20;   // 'a = 10' is redundant
        b = a;
    }
    return 0;
}

