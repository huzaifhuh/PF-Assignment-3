#include <stdio.h>

double totalRepayment = 0.0;

void calculateRepayment(double loan, double rate, int year, int totalYears)
{
    if (year > totalYears || loan <= 0)
    {
        printf("Year %d: Remaining loan = 0.00\n", year);
        return;
    }

    printf("Year %d: Remaining loan = %.2lf\n", year, loan);

    double interest = loan * rate / 100.0;
    loan += interest;

    double installment = 40000.0;
    double paid = (loan < installment) ? loan : installment;

    loan -= paid;
    totalRepayment += paid;

    //recursive call
    calculateRepayment(loan, rate, year + 1, totalYears);
}

int main()
{
    printf("=== Loan Repayment Schedule ===\n");
    printf("Principal: 100000 | Interest Rate: 5%% | Duration: 3 years\n");
    printf("Fixed yearly installment = 40000\n\n");

    totalRepayment = 0.0;
    calculateRepayment(100000.0, 5.0, 1, 3);

    printf("\nLoan repayment completed!\n");
    printf("Total amount repaid by customer = %.2lf\n", totalRepayment);
    printf("Total interest paid = %.2lf\n", totalRepayment - 100000.0);

    return 0;
}