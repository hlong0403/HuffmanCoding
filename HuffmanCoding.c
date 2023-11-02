#include <stdio.h>
#include <stdlib.h> // cung cấp hàm cấp phát bộ nhớ
#include <string.h>
#include <conio.h>

// Node cua cay Huffman
struct node {
    int freq;         //Biến tần số
    char data;        //Biến kí tự a,b,c...
    struct node *left;
    struct node *right;
};

// Tao node moi
struct node *newNode(int freq, char data) 
{
    // Cap phat bo nho cho node
    struct node *temp = (struct node *)malloc(sizeof(struct node));  //cấp 1 danh sách liên kết

    // Gan gia tri cho node
    temp->freq = freq;  //do mình truyền vào
    temp->data = data;  //do mình truyền vào
    temp->left = NULL;
    temp->right = NULL;
    return temp;
}

// In ra cay Huffman
void printHuffmanNode(struct node *root, int level) 
{
    if (root == NULL) return;

    printHuffmanNode(root->right, level + 1);

    for (int i = 0; i < level; i++) printf("\t");
    printf("%d", root->freq);
    if (root->left == NULL || root->right == NULL)
        printf(": %c\n", root->data);
    else 
        printf("\n");
    
    printHuffmanNode(root->left, level + 1);
}

void printHuffmanTree(struct node *root) { //hàm đệ quy
    printf("\nCay Huffman:\n");
    printHuffmanNode(root, 0);
}

// Bieu dien priority queue bang danh sach lien ket
// ---------------------------------------------------------- //
struct listNode {
    struct node *data;
    struct listNode *next;
};

// Tao node cho danh sach lien ket
struct listNode *newListNode(struct node *data) 
{
    struct listNode *temp = (struct listNode *)malloc(sizeof(struct listNode));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

// Chen node moi vao danh sach lien ket ma van giu thu tu tang dan
struct listNode *sortedInsert(struct listNode *head, struct listNode *newNode) 
{
    //TH nút mới nhỏ hơn cả dãy
    if (head == NULL) 
	{   
        newNode->next = head;
        return newNode;
    }

    if (head->data->freq >= newNode->data->freq)
    {
        newNode->next = head;
        return newNode;
    }

    struct listNode *current = head;    // so sánh nút mới với từng nút
    while (current->next != NULL &&
           current->next->data->freq < newNode->data->freq) 
		   {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
    return head;
}

// Lay node dau tien trong danh sach lien ket
struct listNode *getMin(struct listNode *head, struct node **minNode) { //chỗ chứa chứ node nhỏ nhất
    struct listNode *temp = head;   //ngắt date node đầu
    head = head->next;
    *minNode = temp->data;
    free(temp);

    return head;
}
// ---------------------------------------------------------- //

// Cac ham kiem tra input
/*int inRange(char c, char a, char b) { return (c >= a && c <= b); }

int validString(char *str) 
{
    for (int i = 0; i < strlen(str); i++) 
	{
        if (!inRange(str[i], 'a', 'z') && !inRange(str[i], 'A', 'Z') && !inRange(str[i],' ', ' ')) 
		{
            printf("Ky tu khong hop le: \"%c\" o vi tri %d\n", str[i], i);
            return 0;
        }
    }
    return 1;
}*/
// ---------------------------------------------------------- //

// Cac ham xay dung cay Huffman
// ---------------------------------------------------------- //
struct listNode *getFreq(char *str) 
{
    int freq[123] = {0};
    for (int i = 0; i < strlen(str); i++) 
	{
        freq[str[i]]++;
    }

    struct listNode *head = NULL;
    for (int i = 0; i < 123; i++) 
	{
        if (freq[i] > 0) 
		{
            head = sortedInsert(head, newListNode(newNode(freq[i], i)));
        }
    }

    return head;
}

struct node *buildHuffmanTree(char *str) 
{
    struct listNode *head = getFreq(str);

    while (head->next != NULL) {
        struct node *left, *right;
        head = getMin(head, &left);
        head = getMin(head, &right);

        struct node *temp = newNode(left->freq + right->freq, '\0');
        temp->left = left;
        temp->right = right;

        head = sortedInsert(head, newListNode(temp));//Hàm sort ở trên giúp nó tự chui vào 
    }

    struct node *root;
    head = getMin(head, &root);

    return root;
}
// ---------------------------------------------------------- //

// Cac ham ma hoa
// ---------------------------------------------------------- //
void traverseHuffmanTree(struct node *root, char* code, char* codeTable[])    //đi qua từng phần tử của cây
{
    if (root == NULL) return;                // code là để lưu kí tự 0,1 tạm thời khi duyệt cây, còn codeTable là mảng 2 chiều lưu mã hóa 01 của từng kí tự
    if (root->left == NULL && root->right == NULL)              //là node cuối cùng
	{ 
        codeTable[root->data] = malloc(strlen(code) + 1);
        strcpy(codeTable[root->data], code);
        return;
    }

    // Di chuyen sang cay con ben trai, them 0 vao code
    traverseHuffmanTree(root->left, strcat(code, "0"), codeTable);
    // Sau khi di chuyen xong, xoa di ky tu cuoi cung cua code
    code[strlen(code) - 1] = '\0';

    // Di chuyen sang cay con ben phai, them 1 vao code
    traverseHuffmanTree(root->right, strcat(code, "1"), codeTable);
    // Sau khi di chuyen xong, xoa di ky tu cuoi cung cua code
    code[strlen(code) - 1] = '\0';
}

// Tao bang ma tu cay Huffman
void buildCodeTable(struct node *root, char* codeTable[]) 
{
    char code[100] = "";
    traverseHuffmanTree(root, code, codeTable);
}

// In ra bang ma
void printCodeTable(char* codeTable[], FILE* f) {
    fprintf(f, "Bang ma:\n");
    for (int i = 0; i < 123; i++) {
        if (codeTable[i] != NULL) {
            fprintf(f, "%c: %s\n", i, codeTable[i]);
        }
    }
}

// Ma hoa chuoi
void encode(char* str, char* codeTable[], char** code) { // biến code là mảng 2 chiều dùng để lưu mã hóa của các từ mã như a=11, b=10 thì ab=1110
    // Ma hoa chuoi
    for (int i = 0; i < strlen(str); i++) {
        code[i] = malloc(strlen(codeTable[str[i]]) + 1);
        strcpy(code[i], codeTable[str[i]]);
    }
}

// Giai ma 1 ky tu
char decodeChar(char* code, struct node *root) {
    struct node *current = root;
    for (int i = 0; i < strlen(code); i++) {
        if (code[i] == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return current->data;
}

// Giai ma chuoi
void decode(char** code, int len, struct node *root, char* str) {
    for (int i = 0; i < len; i++) {
        str[i] = decodeChar(code[i], root);
    }
    str[len] = '\0';
}

// ---------------------------------------------------------- //

void freeHuffmanTree(struct node *root) 
{
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

int main() {
    char str[100]="";
    
        printf("Nhap vao dia chi file: ");
        char m[100]; //biến dùng để gõ địa chỉ file 
        char hub[100];// dùng để lưu từng kí tự duyệt trong file
        gets(m);
        FILE *fp;
        fp = fopen(m, "r");
        while (fscanf(fp, "%s", hub) != EOF) 
        {
           strcat(str,hub);
        }
        printf("%s",str);
        fclose(fp);
    

    FILE* out_file = fopen("output.text", "w"); //có 3 kiểu mở file write, read, append

    // Viết chuỗi đã nhập vào file
    fprintf(out_file, "Chuoi nhap vao: ");
    fprintf(out_file, "%s\n", str);

    struct node *root = buildHuffmanTree(str);

    printHuffmanTree(root); //muốn in thì dùng ít kí tự

    // Tao bang ma
    char* codeTable[123] = {NULL};
    buildCodeTable(root, codeTable);
    printCodeTable(codeTable, out_file);

    char** code = malloc(strlen(str) * sizeof(char*));

    encode(str, codeTable, code);

    // Luu chuoi da ma hoa vao file
    fprintf(out_file, "Chuoi da ma hoa: \n");
    for (int i = 0; i < strlen(str); i++) {
        fprintf(out_file, "%s ", code[i]);
    }

    // Giai ma chuoi
    char* decodeStr = malloc(strlen(str) + 1);
    decode(code, strlen(str), root, decodeStr);

    // Luu chuoi da giai ma vao file
    fprintf(out_file, "\nChuoi da giai ma: %s\n", decodeStr);

    fclose(out_file);

    // Giai phong bo nho cho bang ma
    for (int i = 0; i < 123; i++) {
        if (codeTable[i] != NULL) {
            free(codeTable[i]);
        }
    }

    // Giai phong bo nho cho chuoi da ma hoa
    for (int i = 0; i < strlen(str); i++) {
        free(code[i]);
    }

    free(code);

    // Giai phong bo nho cho chuoi da giai ma
    free(decodeStr);

    // Giai phong bo nho cho cay Huffman
    freeHuffmanTree(root);

    return 0;
}