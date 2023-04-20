
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
// https://ide2.codingame.com/21140386?id=631645880c43cadc660a6cbb6d614ce7be07f66

int main()
{
	int N, temp, closest_temp = INT_MAX;
	scanf("%d", &N);
	for (int i = 0; i < N; i++)
	{
		scanf("%d", &temp);
		if (abs(temp) < abs(closest_temp) || (abs(temp) == abs(closest_temp) && temp > 0))
		{
			closest_temp = temp;
		}
	}
	if (closest_temp == INT_MAX && closest_temp >= 0)
	{
		printf("0\n");
		return (0);
	}
	printf("%d\n", closest_temp);

	return (0);
}
