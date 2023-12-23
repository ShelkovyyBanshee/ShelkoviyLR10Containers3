#include <iostream>
#include <string.h>
#include <string>

using namespace std;

struct Node
{
    Node()
    {
        value = 0;
        next = NULL;
        previous = NULL;
    }

    Node(int& a)
    {
        value = a;
        next = NULL;
        previous = NULL;
    }

    int value;
    Node* next;
    Node* previous;
};

class Iterator
{
    friend class List;
public:
    Iterator()
    {
        this->el = NULL;
    }

    Iterator(Node* el)
    {
        this->el = el;
    }

    void SetNode(Node* node)
    {
        el = node;
    }

    bool IsEmpty()
    {
        return el == NULL;
    }
    int GetValue()
    {
        return el->value;
    }

    void SetValue(int value)
    {
        el->value = value;
    }

    void MoveNext()
    {
        el = el->next;
    }

    void MovePrevious()
    {
        el = el->previous;
    }
private:
    Node* el;
};

class List
{
public:
    List()
    {
        Head = NULL;
        Tail = NULL;
        begin = Iterator();
        end = Iterator();
        count = 0;
    }

    void Appstart(int element)
    {
        if (Head == NULL)
        {
            Node* firstEl = new Node(element);
            end.SetNode(firstEl);
            begin.SetNode(firstEl);
            Head = firstEl;
            Tail = firstEl;
        }
        else
        {
            Node* newEl = new Node(element);
            Head->previous = newEl;
            newEl->next = Head;
            Head = newEl;
            begin.SetNode(Head);
        }
        count += 1;
    }
    void Append(int element)
    {
        if (Head == NULL)
        {
            Node* firstEl = new Node(element);
            end.SetNode(firstEl);
            begin.SetNode(firstEl);
            Head = firstEl;
            Tail = firstEl;
        }
        else
        {
            Node* newEl = new Node(element);
            Tail->next = newEl;
            newEl->previous = Tail;
            Tail = newEl;
            end.SetNode(Tail);
        }
        count += 1;
    }

    void DeleteLast()
    {
        if (Head == Tail)
        {
            Head = Tail = NULL;
            end = Iterator();
            begin = Iterator();
        }
        else
        {
            Node* newTail = Tail->previous;
            end.MovePrevious();
            delete Tail;
            Tail = newTail;
        }
        count -= 1;
    }

    Iterator Begin() const
    {
        return begin;
    }

    Iterator End() const
    {
        return end;
    }

    int Count() const
    {
        return count;
    }

private:
    Node* Head;
    Node* Tail;
    int count;
    Iterator begin, end;
};

int charToInt(char c)
{
    return c - 48;
}

class Decimal
{
private:
    List list;
public:
    Decimal()
    {
        list = List();
    }

    Decimal(string str)
    {
        list = List();
        int strLen = (int)str.length();
        for (int i = strLen - 1; i >= 0; i--)
        {
            char sym = str[i];
            list.Append(charToInt(sym));
        }
    }

    int Count() const
    {
        return list.Count();
    }

    void Append(int value)
    {
        list.Append(value);
    }
    void Appstart(int value)
    {
        list.Appstart(value);
    }

    bool operator > (const Decimal& other) const
    {
        if (Count() != other.Count()) return Count() > other.Count();

        Iterator iterThis = list.End();
        Iterator iterOther = other.list.End();
        unsigned char valueThis = 0;
        unsigned char valueOther = 0;

        for (int i = 0; i < Count(); i++)
        {
            valueThis = iterThis.GetValue();
            valueOther = iterOther.GetValue();

            if (valueThis != valueOther)
                return valueThis > valueOther;

            iterThis.MovePrevious();
            iterOther.MovePrevious();
        }

        return false;
    }

    bool operator == (const Decimal& other) const
    {
        if (Count() != other.Count()) return false;

        Iterator iterThis = list.Begin();
        Iterator iterOther = other.list.Begin();
        unsigned char valueThis = 0;
        unsigned char valueOther = 0;

        for (int i = 0; i < Count(); i++)
        {
            valueThis = iterThis.GetValue();
            valueOther = iterOther.GetValue();

            if (valueThis != valueOther)
                return false;

            iterThis.MoveNext();
            iterOther.MoveNext();
        }
        return true;
    }

    Decimal operator +(const Decimal& other) const
    {
        if (*this > other) return Add(other);
        else return other.Add(*this);
    }

    Decimal operator -(const Decimal& other) const
    {
        if (*this > other) return Substract(other);
        else return other.Substract(*this);
    }

    Decimal operator *(const Decimal& other) const
    {
        if (*this > other) return Multiply(other);
        else return other.Multiply(*this);
    }

    Decimal operator / (const Decimal& other) const
    {
        return Decimal(to_string(((int)(ToDouble() / other.ToDouble()))));
    }

    long double ToDouble() const
    {
        string str;
        Iterator iterTemp = list.End();
        for (int i = 0; i < Count(); i++)
        {
            str += (char)(48 + iterTemp.GetValue());
            iterTemp.MovePrevious();
        }

        return stod(str);
    }

    void Print() const
    {
        Iterator iter = list.End();
        while (!iter.IsEmpty())
        {
            cout << (int)iter.GetValue();
            iter.MovePrevious();
        }
    }
private:
    Decimal Add(const Decimal& other) const
    {
        Decimal result;
        Iterator iterThis = list.Begin();
        Iterator iterOther = other.list.Begin();
        int valueThis = 0;
        int valueOther = 0;
        int tail = 0;
        int sum = 0;
        for (int i = 0; i < Count(); i++)
        {
            valueThis = iterThis.GetValue();
            iterThis.MoveNext();

            if (!iterOther.IsEmpty())
            {
                valueOther = iterOther.GetValue();
                iterOther.MoveNext();
                sum = tail + valueThis + valueOther;
            }
            else
                sum = tail + valueThis;
            result.Append(sum % 10);
            tail = sum > 9 ? 1 : 0;
        }
        if (tail != 0) result.Append(1);

        return result;
    }

    Decimal Substract(const Decimal& other) const
    {
        Decimal resultTemp;
        Iterator iterThis = list.Begin();
        Iterator iterOther = other.list.Begin();
        int valueThis = 0;
        int valueOther = 0;
        int debt = 0;
        int sum = 0;

        for (int i = 0; i < Count(); i++)
        {
            valueThis = iterThis.GetValue();
            iterThis.MoveNext();
            if (!iterOther.IsEmpty())
            {
                valueOther = iterOther.GetValue();
                iterOther.MoveNext();
                sum = valueThis - valueOther - debt;
            }
            else
                sum = valueThis - debt;

            if (sum < 0)
            {
                sum += 10;
                debt = 1;
            }
            else
                debt = 0;
            resultTemp.Append(sum);
        }

        int count = resultTemp.Count();
        for (int i = 0; i < count && resultTemp.list.End().GetValue() == 0; i++)
        {
            resultTemp.list.DeleteLast();
        }

        if (resultTemp.Count() == 0) resultTemp.Append(0);
        return resultTemp;
    }

    Decimal Multiply(const Decimal& other) const
    {
        Decimal resultTemp;
        for (int i = 0; i < Count() + other.Count(); i++, resultTemp.Append(0));
        Iterator iterThis = list.Begin();
        Iterator iterOther = other.list.Begin();
        Iterator iterResultTemp = resultTemp.list.Begin();
        int valueThis = 0;
        int valueOther = 0;

        for (int i = 0; i < other.Count(); i++)
        {
            valueOther = iterOther.GetValue();
            Iterator iterTemp = iterResultTemp;
            for (int j = 0; j < Count(); j++)
            {
                valueThis = iterThis.GetValue();
                iterTemp.SetValue(iterTemp.GetValue() + valueOther * valueThis);
                iterTemp.MoveNext();
                iterThis.MoveNext();
            }
            iterThis = list.Begin();
            iterResultTemp.MoveNext();
            iterOther.MoveNext();
        }

        iterResultTemp = resultTemp.list.Begin();
        for (int i = 0; i < resultTemp.Count() - 1; i++, iterResultTemp.MoveNext())
        {
            int valueCurrent = iterResultTemp.GetValue();
            iterResultTemp.SetValue(valueCurrent % 10);
            iterResultTemp.MoveNext();

            int valueNext = iterResultTemp.GetValue();
            iterResultTemp.SetValue(valueNext + valueCurrent / 10);

            iterResultTemp.MovePrevious();
        }

        int count = resultTemp.Count();
        for (int i = 0; i < count && resultTemp.list.End().GetValue() == 0; i++)
            resultTemp.list.DeleteLast();
        if (resultTemp.Count() == 0) resultTemp.Append(0);

        return resultTemp;
    }
};

void printDecimalInfo(const Decimal& decimalA, const Decimal& decimalB)
{
    cout << "Decimal1= ";
    decimalA.Print();
    cout << endl;
    cout << "Decimal2= ";
    decimalB.Print();
    cout << endl;
    const int n = 8;
    Decimal one = Decimal("1");
    Decimal results[n] = { decimalA + decimalB, decimalA - decimalB, decimalA * decimalB, decimalA / decimalB,
        decimalA + one, decimalB + one, decimalA - one, decimalB - one };
    string names[n] = { "Decimal1+Decimal2= ", "Decimal1-Decimal2= ", "Decimal1*Decimal2= ", "Decimal1/Decimal2= ",
    "Decimal1++= ", "Decimal2++= ", "Decimal1--= ", "Decimal1--= " };
    if (decimalB > decimalA) names[1] += '-';
    for (int i = 0; i < n; i++)
    {
        cout << names[i];
        results[i].Print();
        cout << endl;
    }
    if (decimalA == decimalB)
        cout << "Decimal1 equal Decimal2";
    else
        cout << "Decimal1 " << (decimalA > decimalB ? "greater" : "less") << " Decimal2" << endl;
}

int main()
{
    int n;
    string strA, strB;
    cin >> n;
    cin >> strA >> strB;
    Decimal decimalA = Decimal(strA), decimalB = Decimal(strB);
    printDecimalInfo(decimalA, decimalB);
    int op = 0;
    int arg = 0;
    while (true)
    {
        cin >> op;
        if (op == 0) break;

        cin >> arg;

        if (op == 1)
        {
            decimalA.Appstart(arg);
        }
        else
            decimalB.Appstart(arg);
        printDecimalInfo(decimalA, decimalB);
    }
}