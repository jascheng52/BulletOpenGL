#include <stdlib.h>
#include <stdio.h>

#include <entity.h>
#include <cglm/call.h>

void test_intersect();
void test_dir();

int main()
{

    test_intersect();
    // test_dir();

}

void test_dir()
{
    vec2 s1, s2, p;
    mat2 d;
    s1[0] = 0, s1[1] = 1;
    s2[0] = -3, s2[1] = 4;

    gen_rot_mat(s1, s2, d);
    glmc_mat2_mulv(d,s1,p);

    printf("x:%f y:%f\n",p[0],p[1]);

}

void test_intersect()
{
    vec2 a1 = {-42.41358,73.55163};
    vec2 a2 = {-1.04299,117.00820};
    vec2 b1 = {50,100};
    vec2 b2 = {-50,100};
    intersect_2d(a1,a2,b1,b2);
    printf("%d\n", intersect_2d(a1,a2,b1,b2));

    // vec2 a1 = {0,0}, a2 = {1,0};
    // vec2 b1 = {0.5,0.5}, b2 = {0.5, -0.5}; 
    // vec2 c1 = {0.1,0}, c2 = {0.3,0}; 
    // vec2 d1 = {-3,-2}, d2 = {-3,5}; 
    // vec2 e1 = {3,1}, e2 = {3,7}; 
    // vec2 f1 = {3,3}, f2 = {-3,3};
    // vec2 g1 = {-3,3}, g2 = {3,-3};
    // vec2 h1 = {-2,1}, h2 = {1,3};
    // vec2 i1 = {-1,6}, i2 = {3,-2};
    // vec2 j1 = {-0.999999,6}, j2 = {1,5};

    // vec2 k1 = {-0.10000,  0.21004 }, k2 = {-0.10000,  0.41004  };
    // vec2 l1 = {-0.10000,  0.40000 }, l2 = {-0.10000,  0.60000 };

    // // printf("Norm: %f\n", glmc_vec2_norm((vec2){1,1}));

    // printf("Expected: tbd Calculated: %d\n", intersect_2d(k1,k2,l1,l2));

    // printf("Expected: 1 Calculated: %d\n", intersect_2d(a1,a2,b1,b2));
    // printf("Expected: 1 Calculated: %d\n", intersect_2d(a1,a2,c1,c2));
    // printf("Expected: 0 Calculated: %d\n", intersect_2d(d1,d2,e1,e2));
    // printf("Expected: 0 Calculated: %d\n", intersect_2d(d1,d2,e1,e2));
    // printf("Expected: 1 Calculated: %d\n", intersect_2d(g1,g2,f1,f2));
    // printf("Expected: 0 Calculated: %d\n", intersect_2d(f1,f2,a1,a2));
    // printf("Expected: 0 Calculated: %d\n", intersect_2d(d1,d2,h1,h2));
    // //based on episolon should be collinear
    // printf("Expected: 1 Calculated: %d\n", intersect_2d(i1,i2,j1,j2));
}
