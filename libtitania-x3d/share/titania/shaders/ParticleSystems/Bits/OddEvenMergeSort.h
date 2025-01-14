/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* Odd even merge sort */

uniform vec4 sortParam;
uniform int  sortSize;

/* Contents of the uniform data fields */
#define TwoStage       sortParam .x
#define Pass_mod_Stage sortParam .y
#define TwoStage_PmS_1 sortParam .z
#define Pass           sortParam .w

float
getDistance (in int);

int
odd_even_merge_sort (in int self)
{
	if (self < sortSize)
	{
		// My position within the range to merge.
		float j = mod (self, TwoStage); //float j = floor (mod (self, TwoStage));

		if ((j < Pass_mod_Stage) || (j > TwoStage_PmS_1))
		{
			// Must copy.
			return self;
		}

		// Must sort.

		bool compare = true;
		int  partner = self;

		if (mod ((j + Pass_mod_Stage) / Pass, 2.0f) < 1.0f)
		{
			// We are on the left side -> compare with partner on the right.
			partner += int (Pass);
		}
		else
		{
			// We are on the right side -> compare with partner on the left.
			compare  = false;
			partner -= int (Pass);
		}

		if (partner < sortSize)
		{
			// On the left its a < operation, on the right its a >= operation.
			if (compare)
				return getDistance (self) < getDistance (partner) ? self : partner;
			else
				return getDistance (self) > getDistance (partner) ? self : partner;
		}
	}

	return self;
}
