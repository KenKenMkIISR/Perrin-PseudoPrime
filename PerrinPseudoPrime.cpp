// PerrinPseudoPrime.cpp
// ペラン擬素数計算

#include <iostream>

#define ARRAYSIZE 360000 // ペラン数を格納する配列の要素数
#define RADIX 0x100000000 // 1要素の最大数+1（符号なし32ビット値）

class Bignum
{
    uint32_t num[ARRAYSIZE]; // 数値を格納する領域
public:
    int digit; // 現在格納されている値の桁数-1
    Bignum(uint32_t n); // コンストラクタ、n:初期値
    bool add(Bignum* B); // 加算
    uint32_t mod(uint32_t n); // 剰余算
};

Bignum::Bignum(uint32_t n) {
    digit = 0;
    num[0] = n;
    for (int i = 1; i < ARRAYSIZE; i++) num[i] = 0;
}

// Bの値を加算する
// 正常終了でtrue、オーバーフローした場合falseを返す
bool Bignum::add(Bignum* B)
{
    int maxdigit = digit;
    if (B->digit>maxdigit) {
        maxdigit = B->digit;
        digit = maxdigit;
    }
    uint32_t c = 0;
    for (int i = 0; i <=maxdigit; i++) {
        uint64_t t = (uint64_t)num[i] + B->num[i] + c;
        num[i] = (uint32_t)t;
        c = (t >= RADIX);
    }
    if (c) {
        digit++;
        if (digit >= ARRAYSIZE) return false; //オーバーフロー
        num[digit] = c;
    }
    return true;
}

// 剰余算
// nで割った余りを返すのみで値は変更しない
uint32_t Bignum::mod(uint32_t n) {
    uint32_t m = 0;
    for (int i = digit; i >= 0; i--) {
        m = ((uint64_t)m * RADIX + num[i]) % n;
    }
    return m;
}

class Bignum p1(3), p2(0), p3(2); // ペラン数列格納オブジェクトに初期値設定

#define MAXNUM 28000000 // エラトステネスのふるいで計算する素数表の大きさ
#define ROOTMAXNUM 5292 // MAXNUMの平方根

uint8_t primenum[MAXNUM / 8 + 1]; // 素数表（ビットフィールド、0が素数）

// xが素数かチェック
// 素数の場合trueを返す
bool testprime(int x)
{
    if (primenum[x / 8] & (1 << (x % 8))) return false;
    return true;
}

// 素数表に合成数xを登録
void setprime(int x)
{
    primenum[x / 8] |= 1 << (x % 8);
}

// エラトステネスのふるいで素数表作成
void Eratosthenes(void)
{
    for (int i = 2; i <= ROOTMAXNUM; i++) {
        if (!testprime(i)) continue;
        for (int j = i * 2; j <= MAXNUM; j += i) {
            setprime(j);
        }
    }
}

void print1(Bignum* A, uint32_t n)
{
    std::cout << n << " is NOT prime number\n";
    std::cout << "maxdigit is " << A->digit << "\n";
}

void print2(Bignum* A, uint32_t n)
{
    std::cout << n << " is prime number\n";
    std::cout << "maxdigit is " << A->digit << "\n";
}

int main()
{
    Eratosthenes(); // 素数表作成

    uint32_t n = 3; // ペラン数列の番号
    while (1) {
        if (!p1.add(&p2)) return n; // P(n)=P(n-2)+P(n-3)、オーバーフローした場合終了
        if (p1.mod(n) == 0) {       // P(n)がnで割り切れた場合
            if (!testprime(n)) print1(&p1, n); // 素数表と比較し、素数でなければペラン擬素数として表示
        }
        else {
            if (testprime(n)) print2(&p1, n); // 割り切れなかった場合にもし素数であれば表示（実際にはない）
        }
        n++;
        if (n > MAXNUM) return n; // 素数表最大値で終了

        // 以下同様にペラン数列を計算
        if (!p2.add(&p3)) return n;
        if (p2.mod(n) == 0) {
            if (!testprime(n)) print1(&p1, n);
        }
        else {
            if (testprime(n)) print2(&p1, n);
        }
        n++;
        if (n > MAXNUM) return n;

        // 以下同様にペラン数列を計算
        if (!p3.add(&p1)) return n;
        if (p3.mod(n) == 0) {
            if (!testprime(n)) print1(&p1, n);
        }
        else {
            if (testprime(n)) print2(&p1, n);
        }
        n++;
        if (n > MAXNUM) return n;
    }
}
