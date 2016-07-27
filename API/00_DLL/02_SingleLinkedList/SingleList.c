// 단순 연결 리스트
#include <stdio.h>
#include <malloc.h>

typedef struct Node{
	int data;
	struct Node *next;
} node;

node *head, *tail; // 머리, 꼬리 전역변수 선언

void init_list(void){ // 초기화
	head = (struct Node*)malloc(sizeof(struct Node));
	tail = (struct Node*)malloc(sizeof(struct Node));
	head->next = tail;
	tail->next = tail;
}

int delete_next(node *t){ // 다음노드 삭제 
	node *s;
	if (t->next == tail){ return 0;}

	s = t->next;
	t->next = t->next->next;
	free(s);
	return 1;
}

node *insert_after(int k, node* t){ // 삽입
	node *s;
	s = (node*)malloc(sizeof(node));
	s->data = k;
	s->next = t->next;
	t->next = s;
	return s;
}

node *find_node(int k){ // 정수로 노드 검색
	node *s;
	s = head->next;
	while (s->data != k && s !=tail){
		s = s->next;
	}
	return s;
}

int delete_node(int k){ // 노드의 삭제
	node *s;
	node *p;
	p = head;
	s = p->next;
	while (s->data != k && s != tail){
		p = p->next;
		s = p->next;
	}
	if (s != tail){
		p->next = s->next;
		free(s);
		return 1;
	}
	else{ return 0;}
}

node *insert_node(int t, int k){ // k전에 t 삽입
	node *s;
	node *p; 
	node *r;
	p = head;
	s = p->next;
	while (s->data != k && s != tail){
		p = p->next;
		s = p->next;
	}
	if (s != tail){ // if find
		r = (node*)malloc(sizeof(node));
		r->data = t;
		p->next = r;
		r->next = s;
	}
	return p->next;
}

node *ordered_insert(int k){
	node *s;
	node *p;
	node *r;
	p = head;
	s = p->next;
	while (s->data <= k && s != tail){
		p = p->next;
		s = p->next;
	}
	r = (node*)malloc(sizeof(node));
	r->data = k;
	p->next = r;
	r->next = s;
	return r;
}

void print_list(node* t){
	printf("\n");
	while (t != tail){
		printf("%-8d", t->data);
		t = t->next;
	}
}

node *delete_all(void){
	node *s;
	node *t;
	t = head->next;
	while (t != tail){
		s = t;
		t = t->next;
		free(s);
	}
	head->next = tail;
	return head;
}

void main(void){
	node* t;

	init_list(); // 초기화

	ordered_insert(10); // 순서를 유지하면서 삽입
	ordered_insert(5);
	ordered_insert(8);
	ordered_insert(3);
	ordered_insert(1);
	ordered_insert(7);
	ordered_insert(8);

	printf("\nInitial Linked list is");
	print_list(head->next);

	printf("\nFinding 4 is %ssucessful", find_node(4) == tail ?"un":"");

	t = find_node(5);
	printf("\nFinding 5 is %ssucessful", t == tail ? "un":"");

	printf("\n");
}          
      
