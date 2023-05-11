typedef int Semaphore;

Semaphore init_sem(char* name, int amount, int initial_value);
Semaphore get_sem(char* name, int amount);

void remove_sem(char* name, int amount);

void increment_sem(Semaphore sema_id, int room_id);
void decrement_sem(Semaphore sema_id, int room_id);
void wait_sem(Semaphore sema_id, int room_id);
