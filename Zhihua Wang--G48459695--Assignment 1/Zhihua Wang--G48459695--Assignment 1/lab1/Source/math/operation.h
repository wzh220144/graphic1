#ifndef __GWU_OPERATION__
#define __GWU_OPERATION__

//2d: calculate the product of (a, b) and (a, c)
float  Xproduct2d(vec3 a, vec3 b, vec3 c) {
	return (b.x -a.x )*(c.y -a.y )-(b.y -a.y )*(c.x -a.x );
}

//2d: calculate the distance between a and b
float Distance2d(vec3 a, vec3 b) {
	return (a.x -b.x )*(a.x -b.x )+(a.y -b.y )*(a.y -b.y );
}

vec3  origin;

bool cmp(const vec3 &a, const vec3 &b) {
	if( Xproduct2d(origin, a, b)==0)
		return Distance2d(origin, a)<Distance2d(origin, b);
	return  Xproduct2d(origin, a, b)>0;
}

void quicksort(vec3 *elem, const int &first, const int &last) {
    int i = first, j = last;
    const vec3 m = elem[(first + last) >> 1];
    vec3 temp;
    do {
        while (cmp(elem[i], m)) i++;
        while (cmp(m, elem[j])) j--;
        if (i <= j) {
            temp = elem[i]; elem[i] = elem[j]; elem[j] = temp;
            i++; j--;
        }
    }while (i < j);
	if (i < last)
		quicksort(elem, i, last);
    if (first < j)
		quicksort(elem, first, j);
}

#endif