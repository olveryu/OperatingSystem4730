#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

#define NUMBER_OF_BIRTHDAY_ELEMENT 100

/*
 * birthday stuct
 */
typedef struct birthday{
  int day;
  int month;
  int year;
  struct list_head list;
}bday;

//declare the head of the list
static LIST_HEAD(birthday_list);

/*
 * this function is to all birthday to the link list
 * @param day: is the day
 * @param month: is the month
 * @param year: is the year
 */
void addBirthday(int day, int month, int year){
  bday* person;
  
  person = kmalloc(sizeof(*person),GFP_KERNEL);
  person->day = day;
  person->month = month;
  person->year = year;
  INIT_LIST_HEAD(&person->list);
  list_add_tail(&person->list, &birthday_list);
}

/*
 * the function is call when the module is loaded
 */
int birthday_init(void){
  bday *ptr;
  int year = 1992;
  int month = 7;
  int day = 25;
  int i = 0;
  for(i = 0; i < NUMBER_OF_BIRTHDAY_ELEMENT; i++){
    addBirthday(day, month, year+i);
  }
  
  // transverse link list
  printk(KERN_INFO "loading module\n");
  list_for_each_entry(ptr, &birthday_list, list){
    printk(KERN_INFO "Birthday: month: %d Day: %d Year: %d\n",
	   ptr->month, ptr->day, ptr->year);
  }
  return 0;
}
/*
 * the function is call when the module is remove
 */
void birthday_exit(void){

  bday *ptr, *next;
  list_for_each_entry_safe(ptr, next, &birthday_list, list){
    printk(KERN_INFO "Birthday: month: %d Day: %d Year: %d\n",
	   ptr->month, ptr->day, ptr->year);
    // delete list
    list_del(&ptr->list);
    // free memory
    kfree(ptr);
  }
  
  printk(KERN_INFO "removing Module\n");
}

module_init(birthday_init);
module_exit(birthday_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("birthday link list");
MODULE_AUTHOR("Zhenyu Yan");

