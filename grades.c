#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grades.h"
#include "linked-list.h"
#define MAX_GRADE 100
//------------------------------------------
struct grades{
	struct list* student_list;
};
//------------------------------------------
struct student_node{ 
	int student_id;
	char *student_name;
	struct list* courses_list;
};
//-----------------------------------------------------------------
struct courses_node{
	char *course_name;
	int course_grade;
};
//------------------------------------------------------------------
struct student_node* create_student_node(const char *name, int id) {
	struct student_node* node = malloc(sizeof(struct student_node));
	node->student_id = id;
	node->student_name = name;
	node->courses_list=NULL;
	if(node==NULL)
		free(node);
	return node;
}
//----------------------------------------------------------------------
struct courses_node* create_courses_node(char *courses_name,int grade) {
	struct courses_node* node = malloc(sizeof(struct courses_node));
	node->course_name = courses_name;
	node->course_grade = grade;
	if(node==NULL)
		free(node);
	return node;
}
//-------------------------------------------------------------------------
struct student_node* find_std_by_id(struct grades *grades,int id){
	struct node* local_it = list_begin(grades->student_list);
	struct student_node* curren_node_ptr = (struct student_node*)list_get(local_it);
	int list_len = list_size(grades->student_list);
	int i=0;
	for (i=0; i<list_len ;i++){
		curren_node_ptr = (struct student_node*)list_get(local_it);
		if(id==(curren_node_ptr->student_id)){
			return curren_node_ptr;// student found
		}
		else
			local_it = (list_next(local_it));
	}
	//the students list is empty 
	return NULL;
}
//--------------------------------------------------------------------
int student_node_clone(void *element, void **output){

	struct student_node *element_as_node = (struct student_node*)element;// casting
	struct student_node **output_casted = (struct student_node**)(output);// casting
	*output_casted = (struct student_node*)malloc(sizeof(struct student_node));
	if(output_casted == NULL) {
		free(output_casted);
		return -1;
	}	
	//clone id
	(*output_casted)->student_id = element_as_node->student_id;
	//clone name
	char *temp_name = (char*)malloc(sizeof(char)*(strlen(element_as_node->student_name)));
    strcpy(temp_name, element_as_node->student_name);
	(*output_casted)->student_name = temp_name;
	//clone courses
	(*output_casted)->courses_list = element_as_node->courses_list;
	free(element_as_node);
	return 0;
}
//--------------------------------------------------------------------
int course_node_clone(void *element, void **output){
	struct courses_node *element_as_node = (struct courses_node*)element;//casting
	struct courses_node **output1_casted = (struct courses_node**)(output);//casting
	*output1_casted = (struct courses_node*)malloc(sizeof(struct courses_node));
	//clone id
	(*output1_casted)->course_grade = element_as_node->course_grade;
	//clone name
	char *temp_name = (char*)malloc(sizeof(char)*(strlen(element_as_node->course_name)));
    strcpy(temp_name, element_as_node->course_name);
	(*output1_casted)->course_name = temp_name;
	//clone courses
	free(element_as_node);
	if(output1_casted != NULL)
		return 0;
	free(output1_casted);
	return -1;	
}
//--------------------------------------------------------------------
void student_node_destroy(void *element){
	struct student_node *element_as_node = (struct student_node*)element;//casting
	free(element_as_node->student_name);
	list_destroy(element_as_node->courses_list);
	//free(element_as_node->courses_list);
	free(element_as_node);
}
//--------------------------------------------------------------------
void course_node_destroy(void *element){
	struct courses_node* element_node = (struct courses_node*)element;//casting
	free(element_node->course_name);
	free(element_node);
}
//-------------------------------------------------------------------------

struct grades* grades_init(){
	struct grades* student_grd_lst = (struct grades*)malloc(sizeof(struct grades));
	student_grd_lst->student_list = list_init(student_node_clone,student_node_destroy);
	return student_grd_lst;
}
//-------------------------------------------------------------------------

void grades_destroy(struct grades *grades){
	list_destroy(grades->student_list);
	free(grades);
}
//-------------------------------------------------------------------------

int grades_add_student(struct grades *grades, const char *name, int id){
	struct node* std_iterator;
	std_iterator = list_begin(grades->student_list);
	if(std_iterator != NULL){//there is at least 1 student node
		struct student_node* curren_node_ptr = (struct student_node*)list_get(std_iterator);
		int list_len = list_size(grades->student_list);
		int i=0;
		for (i=0; i<list_len ;i++){
			curren_node_ptr = (struct student_node*)list_get(std_iterator);
			if(id==(curren_node_ptr->student_id)){
				return 1;//already exsisted
			}
			else
				std_iterator = list_next(std_iterator);
		}//there is no node with that ID, add it
	}
	//the students list is empty OR we need to add a new node
	struct student_node* new_node = create_student_node(name,id);
	return list_push_back(grades->student_list, new_node);
}
//-------------------------------------------------------------------------

int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade){
	 if(grade<0 || grade>MAX_GRADE)
		 return 1;//the grade invalid
	 if (grades==NULL)//"grades" is invalid
		return 1;
	 struct node* std_iterator;
	 struct node* course_iterator;	 
	 std_iterator = list_begin(grades->student_list);
	 if(std_iterator != NULL){                            //there is at least 1 student
		struct student_node* curren_node_ptr = (struct student_node*)list_get(std_iterator);
		int list_len = list_size(grades->student_list);
		for (int i=0; i<list_len ;i++){      // run for all students
			curren_node_ptr = (struct student_node*)list_get(std_iterator);
			if(id==(curren_node_ptr->student_id)){     //we found the student
				if((curren_node_ptr->courses_list) == NULL){     //enter if the student does not have a courses list
					curren_node_ptr->courses_list = list_init(course_node_clone,course_node_destroy);
					struct courses_node* new_node = create_courses_node(name,grade);
					return list_push_back(curren_node_ptr->courses_list,new_node);
				}
				else{   //if we got here, we already have a list of courses
					    // search for the same course in the list:
					course_iterator = list_begin(curren_node_ptr->courses_list);
					struct courses_node* curren_course_ptr = (struct courses_node*)list_get(course_iterator);
					int course_list_len = list_size(curren_node_ptr->courses_list);
					for (int j=0; j<course_list_len ;j++){    // run for all courses
						curren_course_ptr = (struct courses_node*)list_get(course_iterator);
						if((strcmp(name,(curren_course_ptr->course_name))) == 0 ){ //we already have this course in the courses list
							 return 1;//already existed
						}
						else 
							course_iterator = list_next(course_iterator);
					}  //there is not a node with that course_name, add it 
					struct courses_node* new_node = create_courses_node(name,grade);
					return list_push_back((curren_node_ptr->courses_list),new_node);
				}					
			}
			else
				std_iterator = list_next(std_iterator);
		}    //there is no student with that ID, failed
		
	}
	return 1;
}
//-------------------------------------------------------------------------

float grades_calc_avg(struct grades *grades, int id, char **out){
	struct student_node* student = find_std_by_id(grades,id);
	//edges cases:
	if (grades==NULL || student == NULL )
		return -1;
	int list_len = list_size(student->courses_list);
	int total_grades = 0;
	float avg_grade = 0.00;
	struct node* course_iterator;
	course_iterator = list_begin(student->courses_list);
	struct courses_node* curren_course_ptr = (struct courses_node*)list_get(course_iterator);
	for (int i=0; i<list_len ;i++){      // run for all courses
		curren_course_ptr = (struct courses_node*)list_get(course_iterator);
		total_grades += curren_course_ptr->course_grade;
		course_iterator = list_next(course_iterator);
	} 
	if(list_len != 0)
		 avg_grade = (((float)total_grades)/list_len);
	char *temp_name = (char*)malloc(sizeof(char)*(strlen(student->student_name)));//clone the student's name:
    strcpy(temp_name,student->student_name);
	(*out) = temp_name;
	return avg_grade;
}	
//-------------------------------------------------------------------------

int grades_print_student(struct grades *grades, int id){
	struct student_node* student = find_std_by_id(grades,id);
		//edges cases:
	if (grades==NULL || student == NULL)
		return -1;
	printf("%s %d:",student->student_name,student->student_id);//print student name and ID
	struct node* course_iterator;
	course_iterator = list_begin(student->courses_list);
	int list_len = list_size(student->courses_list);
	struct courses_node* curren_course_ptr = (struct courses_node*)list_get(course_iterator);
	for (int i=0; i<list_len ;i++){    // run for all courses
		curren_course_ptr = (struct courses_node*)list_get(course_iterator);
		if(i < (list_len-1))
			printf(" %s %d,",curren_course_ptr->course_name,curren_course_ptr->course_grade);
		else
			printf(" %s %d",curren_course_ptr->course_name,curren_course_ptr->course_grade);
		course_iterator = list_next(course_iterator);
	}  
	printf("\n");
	return 0;
}
//-------------------------------------------------------------------------
int grades_print_all(struct grades *grades){
	if (grades==NULL) 
		return -1;
	struct node* std_iterator;
	std_iterator = list_begin(grades->student_list);
	struct student_node* curren_node_ptr = (struct student_node*)list_get(std_iterator);
	int list_len = list_size(grades->student_list);
	int i=0;
	for (i=0; i<list_len ;i++){
		curren_node_ptr = (struct student_node*)list_get(std_iterator);
		grades_print_student(grades,(curren_node_ptr->student_id));
		std_iterator = list_next(std_iterator);
	}
	return 0;
}


 









