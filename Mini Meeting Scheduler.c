#include <stdio.h>

typedef struct {
    int hour;
    int minute;
} Time;

Time start = {9, 0};  // Available from 9:00 AM
Time end = {17, 0};   // Available until 5:00 PM

int time_to_minutes(Time t) {
    return t.hour * 60 + t.minute;
}

void minutes_to_time(int mins, Time *t) {
    t->hour = mins / 60;
    t->minute = mins % 60;
}

void suggest_meeting() {
    int duration;
    printf("Enter meeting duration (minutes): ");
    scanf("%d", &duration);
    
    int start_mins = time_to_minutes(start);
    int end_mins = time_to_minutes(end);
    
    if (start_mins + duration > end_mins) {
        printf("Not enough time today! Available: %d:%02d to %d:%02d\n", 
               start.hour, start.minute, end.hour, end.minute);
        return;
    }
    
    Time suggested;
    minutes_to_time(start_mins, &suggested);
    printf("Earliest meeting: %d:%02d for %d minutes\n", 
           suggested.hour, suggested.minute, duration);
    
    // Update start time
    start_mins += duration;
    minutes_to_time(start_mins, &start);
    printf("Next available slot: %d:%02d\n", start.hour, start.minute);
}

int main() {
    printf("Work hours: 9:00 AM - 5:00 PM\n");
    int choice;
    do {
        printf("\n1. Schedule Meeting\n2. Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) suggest_meeting();
    } while(choice != 2);
    return 0;
}