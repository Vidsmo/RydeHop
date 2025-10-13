// payment.h
#ifndef PAYMENT_H
#define PAYMENT_H

#include <time.h>
#include "ride.h"

typedef enum {
    PAYMENT_PENDING,
    PAYMENT_COMPLETED,
    PAYMENT_FAILED,
    PAYMENT_REFUNDED
} PaymentStatus;

typedef struct {
    int id;
    Ride *ride;
    double amount;
    char *payment_method;
    PaymentStatus status;
    time_t timestamp;
    char transaction_id[30];
} Payment;

Payment* create_payment(int id, Ride *ride, double amount, const char *method);
void process_payment(Payment *payment);
void refund_payment(Payment *payment);
void print_receipt(const Payment *payment);
int validate_payment_method(const char *method);
void log_payment(const Payment *payment);
void free_payment(Payment *payment);

#endif
