#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


typedef struct  {
    int x,y;

} fraction;
void ni();

int gcd(int x,int y){
    return y==0?x:gcd(y,x%y);
}//gcd

void reduction(int* x,int* y) {
    int cnt=( *x < 0)+(*y < 0);//the number of (<0)
    if (*x < 0) {
        *x *= -1;
    }
    if (*y<0) {
        *y *= -1;
    }
    int GCD=gcd(*x,*y);
    *x /= GCD;
    *y /= GCD;
    if (cnt &1) {//even &1=0,odd&1=1
        *x *=-1;
    }
}


fraction add(fraction* a,fraction*b){//equals "+"
    fraction result={
        a->x*b->y+a->y*b->x,
        a->y*b->y
    };
    reduction(&result.x,&result.y);
    return result;
}

fraction dec(fraction* a,fraction*b){//equals "-"
    fraction result={
        a->x*b->y-a->y*b->x,
        a->y*b->y
    };
    reduction(&result.x,&result.y);
    return result;
}

fraction mul(fraction*a, fraction*b){//equals "*"
    fraction result={
        a->x*b->x,
        a->y*b->y
    };
    reduction(&result.x,&result.y);
    return result;
}

fraction divide(fraction*a, fraction*b){//equals "/"
    fraction result ={
        a->x*b->y,
        a->y*b->x
    };
    reduction(&result.x,&result.y);
    return result;
}



typedef struct Matrix {//declare Matrix;
    int row,col;
    fraction value[100][100];
}Matrix;


void print_fraction(fraction a){
    if (a.y==1) {
        printf("%d",a.x );
    }
    else printf("%d/%d",a.x,a.y);
}

int print_matrix(Matrix * mat){
    int i,j;

    printf("MAtrix:\n");//for beauty;
    for (i = 0; i < mat->row; i++) {
        for ( j = 0; j < mat->col; j++) {
        //this part can change into a function;
        if(mat->value[i][j].x>=0)printf(" ");

        print_fraction(mat->value[i][j]);

        if(mat->value[i][j].y==1)printf("    ");
        else printf("  ");
        }
        printf("\n");
    }
    return 0;
}

Matrix * read(){//input Matrix;
    int i,j;

    static struct Matrix mat;
    printf("input the numbers of rows and cols\n" );
    scanf("%d%d",&mat.row,&mat.col );//input row and col;
    printf("Input the matrix or determinant\n");
    for ( i = 0; i < mat.row; i++) {
        for(  j=0;j<mat.col;j++){
            scanf("%d",&mat.value[i][j].x);
            if (getchar()=='/') {//fraction or not?
                scanf("%d",&mat.value[i][j].y);
            }
            else{
                mat.value[i][j].y=1;//denominator=1;
            }
        }
    }

    return &mat;
}


void swap_row(Matrix * mat,int row1,int row2) {
    int i,j;

    for ( i = 0; i <mat->col; i++) {
        fraction trm;
        trm=mat->value[row1][i];
        mat->value[row1][i]=mat->value[row2][i];
        mat->value[row2][i]=trm;
    }

}


//阶梯矩阵，对角矩阵
Matrix * cha_into_dia(Matrix * mat){
    int m=0;
    int i,k,j;

    // print_matrix(mat);
    for (i = 0; i < mat->row; i++) {
        int ok=1;

        if (mat->value[i][m].x==0) {
            ok=0;
            for (k = i+1; k < mat->row; k++) {
                if (mat->value[k][m].x!=0) {
                    ok=1;
                    swap_row(mat,i,k);
                    break;
                }
            }
        }

        if (ok==0) {
            i--;
            m++;
            if(m>=mat->col){
                break;
            }
            continue;
        }


        for ( j = 0; j <mat->row; j++) {
            if (mat->value[j][m].x==0||i==j) {
                continue;
            }
            fraction b;
            // printf("%d\n",i);
            // print_fraction(mat->value[j][m]);
            b=divide(&mat->value[j][m],&mat->value[i][m]);
            for ( k = m; k <mat->col; k++) {
                fraction MUL=mul(&b,&mat->value[i][k]);
                mat->value[j][k]=dec(&mat->value[j][k],&MUL);
            }
        }
        m++;
        if(m>=mat->col){
            break;
        }
        // print_matrix(mat);
    }

    return mat;
}



//计算特征值和特征向量 雅克比迭代法
bool jacbi(Matrix *mat ,Matrix * dblVect,double *pdbEigenValue){
    int i,j;

    for(i=0;i<mat->col;i++){
        // dblVect->value[]
    }
}



//计算行列式
fraction  cal_det(Matrix * mat){
    // printf("det?" );
    int i,j;

    Matrix mat_bak;
    for( i=0;i<mat->row;i++){
        for( j=0;j<mat->col;j++){
            mat_bak.value[i][j]=mat->value[i][j];
        }
    }

    mat=cha_into_dia(mat);//here!!!!
    for ( i = 0; i < mat->row; i++) {
        if (mat->value[i][i].x==0) {
            fraction ans;
            ans.x=0;
            ans.y=1;
            return ans;
        }
    }
    // printf("%d",mat_bak.value[0][1].x);
    fraction an;//here is wrong
    an.x=1;
    an.y=1;

    for ( i = 0; i <mat->row; i++) {
        an=mul(&an,&mat->value[i][i]);
    }
    for( i=0;i<mat->row;i++){
        for( j=0;j<mat->col;j++){
            mat->value[i][j]=mat_bak.value[i][j];
        }
    }
    return an;
}


int is_singular(Matrix * mat){

    fraction an=cal_det(mat);
    if (an.x==0) {
        // printf("singular wrong?");
        return 1;
    }
    return 0;
}


Matrix * get_inverse(Matrix * mat){
    //row!=col
    int i,j;

    if (mat->row!=mat->col) {
        printf("row!=col,not a matrix!");
        exit(0);
    }

    //is singular or not?
    if (is_singular(mat)) {
        printf("This matrix is a singular one which is not inversable.\n" );
        exit(0);
    }
    else{//inversable
        //
        for ( i = 0; i <mat->row; i++) {
            for (j = 0; j <mat->col; j++) {
                mat->value[i][j+mat->col].y=1;
                if (i==j) {
                    mat->value[i][j+mat->col].x=1;
                }
                else mat->value[i][j+mat->col].x=0;
            }
        }

        mat->col*=2;
        // print_matrix(mat);
        mat=cha_into_dia(mat);
        // print_matrix(mat);
        mat->col/=2;
        for ( i = 0; i < mat->row; i++) {
            fraction b;
            b=mat->value[i][i];
            for (j =0; j <mat->col; j++) {
                mat->value[i][j]=divide(&mat->value[i][j+mat->col],&b);
            }
        }
    }
    return mat;
}

//求秩
int  rank(Matrix * mat){
    int i,j;

    mat=cha_into_dia(mat);
    print_matrix(mat);

    int n=0;
    int ok=0;

    for( i=0;i<mat->row;i++){
        ok=0;
        for ( j = 0; j <mat->col; j++) {
            if (mat->value[i][j].x!=0) {
                n++;
                ok=1;
                break;
            }

        }
        if (ok==0) {
            return n;
        }
    }
}

//求齐次方程的基础解系
#define MAXN 100
#define MAX 100
int  syst(Matrix * mat){
    cha_into_dia(mat);

    int i,j;
    fraction x[MAXN][MAX];
    int fuzhi[mat->col],pos[mat->row];

    j=0;
    for(i=0;i<mat->row;i++){
        for(j;j<mat->col;j++){
            //非自由未知量
            if(mat->value[i][j].x!=0){
                fuzhi[j]=1;
                pos[j]=i;
                j++;
                break;
            }
            //自由未知量
            else {
                fuzhi[j]=0;

            }
        }
    }

    for(i=0;i<mat->col;i++){
        if(fuzhi[i]!=1){
            x[i][i].x=1;
            x[i][i].y=1;
            for(j=0;j<mat->col;j++){
                if (i==j) {
                    continue;
                }
                //
                else if(fuzhi[j]==1){
                    x[i][j] = divide(&mat->value[pos[j]][i],&mat->value[pos[j]][j]);
                    x[i][j].x = -x[i][j].x;
                }
                else{
                    x[i][j].x = 0;
                    x[i][j].y = 1;
                }
            }
        }
    }
    int m=1;
    printf("基础解系为：\n");
    for(i=0;i<mat->col;i++){
        if(fuzhi[i]==0){
            printf("X%d:(",m);
            m++;
            for(j=0;j<mat->col;j++){
                print_fraction(x[i][j]);
                if(j!=mat->col-1){
                    printf(" , " );
                }
                else printf(")\n" );
            }
        }
    }
    return m;
}

//齐次线性方程组

void linear_ab(Matrix *mat){
    int i,j;
    fraction x[MAXN][MAX];
    int fuzhi[mat->col],pos[mat->row];
    //mat为系数矩阵
    if(rank(mat)==mat->col){
        //只有零解
        printf("x = 0\n" );
    }
    else {
        cha_into_dia(mat);

        int m;//解向量数量
        m=syst(mat);
        printf("一般解为：\n  X = ");
        for(i=1;i<m;i++){
            printf("k%d X%d",i,i);
            if(i!=m-1){
                printf(" + ");
            }
            else printf("\n");
        }
    }
}

//求解非齐次线性方程组
void linear_nor(Matrix *mat){
    int i,j;

    Matrix co_mat;//coefficient matrix;
    co_mat.row=mat->row;
    co_mat.col=mat->col-1;

    for (  i = 0; i <co_mat.row; i++) {
        for ( j = 0; j <co_mat.col; j++) {
            co_mat.value[i][j]=mat->value[i][j];

        }
    }

    int r1=rank(mat);

    int r2=rank(&co_mat);


    if (r1!=r2) {
        printf("no answer!");
    }

    //只有唯一解，b=0时，只有零解
    else if (r1==r2&&r1==co_mat.col) {
        cha_into_dia(mat);
        for (i=0;i<mat->col-1;i++) {
            fraction ans;
            ans = divide(&mat->value[i][mat->col-1],&mat->value[i][i]);
            printf("x%d = ",i+1 );
            print_fraction(ans);
            printf("\n");
        }

    }

    //无穷多个解
    else {
        cha_into_dia(mat);
        //特解
        printf("特解为：\n   X0 = (" );
        fraction x0[mat->col];
        int fuzhi[mat->col];
        memset(fuzhi,0,mat->col);
        x0[0].x=0;
        j=0;
        for(i=0;i<mat->row;i++){
            for(j;j<mat->col-1;j++){
                if (mat->value[i][j].x!=0) {
                    x0[j]=divide(&mat->value[i][mat->col-1],&mat->value[i][j]);
                    fuzhi[j]=1;
                    j++;
                    print_fraction(x0[j]);
                    if(j!=mat->col-2){
                        printf(" , " );
                    }
                    else printf(")\n" );
                    break;
                }
                else  {
                    x0[j].x=0;
                    x0[j].y=1;
                    fuzhi[j]=0;
                }
                print_fraction(x0[j]);
                if(j!=mat->col-2){
                    printf(" , " );
                }
                else printf(")\n" );
            }
            if(j==mat->col-2) break;
        }

        //AX=0的基础解系
        cha_into_dia(&co_mat);
        int m = syst(&co_mat);

        //一般解
        printf("一般解为：\n  X = x0 ");
        for(i=1;i<m;i++){
            printf("+ k%dX%d",i,i );
            if (i==m-1) {
                printf("\n");
            }
        }

    }
}




//输入提示
int print_tips(){
    printf("This is a Matrix operator.\n");
    printf("1   matrix inversion(矩阵求逆)\n");
    printf("2   caculate detaminate(计算行列式)\n" );
    printf("3   linear equations(求解线性方程组)\n" );
    printf("4   find the rank(求秩)\n" );
    printf("5   向量运算\n");
    printf("Please input the number before your wanting operation.\n" );
    //waiting to add function;
    int n;
    scanf("%d",&n);
    return n;
}

int operations(int n) {
    if (n==1) {
        Matrix *mat=read();
        get_inverse(mat);

        print_matrix(mat);
    }
    if (n==2) {
        Matrix *mat=read();
        fraction ans;
        ans=cal_det(mat);
        print_fraction(ans);
    }
    if (n==3) {//to do :
        printf("非齐次线性方程输1\n");
        int choose;
        scanf("%d",&choose);

        if(choose==1){
            printf("Input the augmented matrix(增广矩阵)\n" );
            Matrix *mat=read();
            linear_nor(mat);
        }

        else {
            printf("输入系数矩阵\n" );
            Matrix *mat=read();
            linear_ab(mat);

        }
    }
    if (n==4) {
        Matrix *mat=read();
        int ans=rank(mat);
        printf("%d\n",ans );
    }
    if (n==5) {
        /* code */
    }
    if (n==-1) {
        exit(0);
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    while(1)
        operations(print_tips());
    system("PAUSE");
    return 0;
}
