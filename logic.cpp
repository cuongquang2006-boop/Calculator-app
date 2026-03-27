#include <iostream>
#include <string>
#include <stack>
using namespace std;

int sosanhTT(char op) 
{
    if(op == '+' || op == '-') return 1;
    if(op == '*' || op == '/') return 2;
    return 0;
}

double tinhcapPT(double a, double b, char c) 
{
    switch (c)
    {
        case '+': return a+ b;
        case '-': return a- b;
        case '*': return a* b;
        case '/': 
            if(b == 0) cout << "Loi, khong the chia cho 0" << endl; return 0;
            return a / b;
    }
    return 0;
}

double tinhBT(string s)
{
    stack<double>v;
    stack<char>ops;

    for(int i = 0 ; i < s.size(); i++)
    {
        if(s[i] == ' ') continue;

        if(s[i] >= '0' && s[i] <= '9') 
        {
            double val = 0;
            while(i < s.length() && s[i] >= '0' && s[i] <= '9')
            {
                val = val * 10 + (s[i] - '0');
                i++;
            }
            v.push(val);
            i--; //lay toan tu
        }

        else if(s[i] == '(')
        {
            ops.push(s[i]);
        }
        
        else if(s[i] == ')') 
        {
            while(ops.empty() == false && ops.top() != '(')
            {
                double b = v.top(); v.pop();
                double a = v.top(); v.pop();
                char c = ops.top(); ops.pop();
                v.push(tinhcapPT(a,b,c));
            }
            if(ops.empty() == false) ops.pop();
        }

        else  //tim thay dau phep tinh
        {
            while(ops.empty() == false && sosanhTT(ops.top()) >= sosanhTT(s[i]))
            {
                double b = v.top(); v.pop();
                double a = v.top(); v.pop();
                char op = ops.top(); ops.pop();
                v.push(tinhcapPT(a,b,op));
            }
            ops.push(s[i]); // them phan tu
        }
    }
    while(ops.empty() == false)
    {
        double b = v.top(); v.pop();
        double a = v.top(); v.pop();
        char op = ops.top(); ops.pop();
        v.push(tinhcapPT(a,b,op));
    }

    return v.top();
}


int main()
{
    string s;
    cout << "Nhap vao bieu thuc: " << endl;
    getline(cin, s);

    cout << "Ket qua: " << tinhBT(s) << endl;
    
    return 0;
}
