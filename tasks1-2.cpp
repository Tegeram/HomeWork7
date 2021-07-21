#include <iostream>

using namespace std;

//=====Exercise 1================================================================================================

class Date {
private:
    int m_day, m_month, m_year;
public:
    Date() {
        m_day = 1; m_month = 1; m_year = 2020;
    }
    
    void setDate(int day, int month, int year) {
        m_day = day;
        m_month = month;
        m_year = year;
    }

    int getDay() const {
        return m_day;
    }

    int getMonth() const {
        return m_month;
    }

    int getYear() const {
        return m_year;
    }

    friend ostream& operator<< (ostream& out, const Date& date);
};

ostream& operator<< (ostream& out, const Date& date)
{
    out << "Date: " << date.getDay() << ". " << date.getMonth() << ". " << date.getYear() << endl;
    return out;
}

template<typename T>
void isEmpty(unique_ptr<T>& ptr) {
    if (ptr) cout << "not empty" << endl;
    else cout << "empty" << endl;
}

//=====Exercise 2===========================================================================================

void compareDates(unique_ptr<Date>& ptr1, unique_ptr<Date>& ptr2) {
    if (ptr1->getYear() > ptr2->getYear()) cout << *ptr1;
    else {
        if ((ptr1->getYear() == ptr2->getYear()) && (ptr1->getMonth() > ptr2->getMonth())) cout << *ptr1;
        else {
            if (ptr1->getYear() == ptr2->getYear() && ptr1->getMonth() == ptr2->getMonth() && ptr1->getDay() > ptr2->getDay()) cout << *ptr1;
            else cout << *ptr2;
        }
    }
}

void swapDate(unique_ptr<Date>& ptr1, unique_ptr<Date>& ptr2) {
    unique_ptr<Date> ptr3 = make_unique<Date>();
    ptr3 = move(ptr1);
    ptr1 = move(ptr2);
    ptr2 = move(ptr3);    
}

//=====================================================================================================

int main()
{    
    //=====================================================================================================

    cout << "Ex 1" << endl;
    unique_ptr<Date> today = make_unique<Date>();
    unique_ptr<Date> date = make_unique<Date>();    
    today->setDate(21, 1, 2021);
    cout << today->getDay() << "." << today->getMonth() << "." << today->getYear() << endl;
    cout << *today;
    cout << *date;    
    isEmpty(today);
    isEmpty(date);
    date = move(today);
    isEmpty(today);
    isEmpty(date);    
    cout << *date << endl;

    //=====================================================================================================

    cout << "Ex 2" << endl;
    unique_ptr<Date> date1 = make_unique<Date>();
    unique_ptr<Date> date2 = make_unique<Date>();
    date1->setDate(15,1,2020);
    cout << *date1 << *date2;

    compareDates(date1, date2);

    swapDate(date1, date2);

    cout << *date1 << *date2;


    return 0;
}

