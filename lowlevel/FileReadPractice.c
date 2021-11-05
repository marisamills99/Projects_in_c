 #include<stdio.h>
 typedef struct {int x; char v; } rec_t;
 int main() { int c; FILE *fp; rec_t my; fp=fopen("test.bin","wb");
 for ( c=1; c <= 10; c++) { my.x= c; my.v= c+64;
 fwrite(&my, sizeof(rec_t), 1, fp); } fclose(fp);
 printf("%d; %d; %d; %d; %d; %c\n",
 sizeof(my), sizeof(rec_t), sizeof(my.x), sizeof(my.v), my.x, my.v);
 
 fp=fopen("test.bin","rb");

 fread(&my, sizeof(rec_t), 1, fp); printf("(%d, %c); ", my.x, my.v);
 fseek(fp, 5*sizeof(rec_t), SEEK_SET);
 fread(&my, sizeof(rec_t), 1, fp); printf("(%d, %c); ", my.x, my.v);
 fseek(fp, 2*sizeof(rec_t), SEEK_CUR);
 fread(&my, sizeof(rec_t), 1, fp); printf("(%d, %c); ", my.x, my.v);
 fseek(fp, -16, SEEK_END);
 fread(&my, 8, 1, fp); printf("(%d, %c); ", my.x, my.v);
 rewind(fp);
 fread(&my, sizeof(rec_t), 1, fp); printf("(%d, %c); ", my.x, my.v);
 fclose(fp); 
 
 return 0; }