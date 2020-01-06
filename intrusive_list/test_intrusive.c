

#include "intrusive_list.h"

struct list_head {
	struct list_head *next, *prev;
};


typedef struct  {
     int data ;
     struct list_head mylist ;
} mystruct;



struct list_head *head = NULL;

void main()
{
 mystruct first;
 mystruct second;
 mystruct  third;
 mystruct zero;

 first.data = 10;
 second.data = 20;
 third.data = 30;
 zero.data = 0;

printf("starting linked list\n");

 CDL_APPEND(head, &zero.mylist);  // This will be my head and its data is invalid
 CDL_APPEND(head, &first.mylist);
 CDL_APPEND(head, &second.mylist);
 CDL_APPEND(head, &third.mylist);

printf("Printing the list:\n");
struct list_head *position; 
 mystruct  *datastructureptr  = NULL ;
   
     CDL_FOREACH(position, &zero.mylist)
    { 
         datastructureptr = list_entry(position, mystruct , mylist ); 
         printf ("data  =  %d\n" , datastructureptr->data ); 
    }


 struct list_head* pos;
    CDL_FOREACH(pos , &zero.mylist )
    { 
        printf ("surfing the linked list next = %p and prev = %p\n" , 
             pos->next, 
             pos->prev );
    }
   int count =0;
   CDL_COUNT(pos,head, count);
   printf("Count of elements = %d\n", count );

   printf("Deleting an element:\n");
   CDL_DELETE(head, &second.mylist);

   printf("Printing list after deleting:\n");
   CDL_FOREACH(position, &zero.mylist)
  {
       datastructureptr = list_entry(position, mystruct , mylist );
       printf ("data  =  %d\n" , datastructureptr->data );
  }

   printf("Appending an element that was deleted:\n");
   CDL_APPEND_ELEM(head, &first.mylist, &second.mylist);

   printf("Printing list after appending:\n");
   CDL_FOREACH(position, &zero.mylist)
  {
       datastructureptr = list_entry(position, mystruct , mylist );
       printf ("data  =  %d\n" , datastructureptr->data );
  }


}
