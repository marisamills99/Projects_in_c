/* Fill in your Name and GNumber in the following two comment fields
 * Name: Marisa Mills
 * GNumber: 01075052
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fp.h"
typedef struct fp_struct {
  int sign;   
  int exp; 
  int frac; 
}Fp;
/* input: float value to be represented
 * output: integer representation of the input float
 *
 * Perform this the same way we did in class -
 *   either dividing or multiplying the value by 2
 *   until it is in the correct range (between 1 and 2).
 *
 * Follow the Project Documentation for Instructions
 */
 Fp check_fp(int exp, int sign, int frac){
	Fp myfp;
	//check for underflow or zero both pos and neg
	 if (exp<=0 ){
		if(sign==0){
			myfp.sign= 1;
			myfp.exp=0;
			myfp.frac=0;
			return myfp;
		}
		else{
			myfp.sign= -1;
			myfp.exp=0;
			myfp.frac=0;
			return myfp;
		}
	}
	//check for overflow or pos or neg infinity 
	if (exp>14){
		if(sign==0){
			myfp.sign= 0;
			myfp.exp=15;
			myfp.frac=0;
			return myfp;
		}
		else{
			myfp.sign= 1;
			myfp.exp=15;
			myfp.frac=0;
			return myfp;
		}
	}
	//otherwise return same num
	myfp.sign= sign;
	myfp.exp=exp;
	myfp.frac=frac;
	return myfp;
}
 int round_frac(float frac){
	 int x;
	//check round to even
	int y= frac;
	if (frac-y==0.5){
      //round even 
      int z= frac-0.5;
      if (z %2==0){
        //z is even
        return z;
      }
    }
	//add .5 then truncate
	 x = (frac+0.5)*1;
	return x;
	
}
 //overflow only  1111 000000
 //return INFINITY /-INFINITY
int compute_fp(float val) {
  int scale=0;
  int sign=0;
  int mynum=0;
  int frac=0;
  int exp; 
  // check sign
	if(val<0){
		sign=1;
		val = val*-1;
	}
	//check for zero
	if (val==0){
		sign= 1;
		exp= 0;
		frac= 0;
		mynum= packint(sign,exp,frac);
		return mynum;
		
	}
  //move decimal right
  if (val<1){
	  while (val<1){
		  scale--;
		  val =val *2;
	  }
  }
  //move the decimal left
  else if (val>=2){
	  while (val >=2){
		scale++;
		val=val/2;
	  }
  }
	float x = val-1;
	x *= 64;
	// frac is a whole int that when / by 2^6 is a fractional number
	frac= round_frac(x);
	
  //bias = 2^e-1  - 1 = 7 
	exp = scale + 7;
  //check underflow &over flow
	Fp myfp=check_fp(exp,sign,frac);
	sign= myfp.sign;
	exp= myfp.exp;
	frac= myfp.frac;
  
  // pack the number
	mynum= packint(sign,exp,frac);
	  
  return mynum;
}

int packint(int sign, int exp, int frac){
	// shift then OR to pack the number
	int mynum =0;
	mynum = mynum|sign;
	mynum = mynum <<4;
	mynum = mynum|exp;
	mynum = mynum <<6;
	mynum = mynum|frac;
	return mynum;
}
/* input: integer representation of our floats
 * output: float value represented by our integer encoding
 *
 * Follow the Project Documentation for Instructions
 */


Fp get_struct_fp(int val){
	Fp newfp;

	int sign=0;
	int mynum=0;
	int tworaisedE=1;
	// use masking to get correct bits
	
	//hex 400 =(100 0000 0000)
	sign=(val& 0x400)>>10;
	// change sign  to represent -1^0 or -1^1
	if (sign== 1){
		newfp.sign=-1;
	}
	else{
		newfp.sign=1;
	}
	//hex 3C0=(011 1100 0000)
	newfp.exp =(val& 0x3C0)>>6;
	//hex 3F= (000 0011 1111)
	newfp.frac=val& 0x3F;

	return newfp;
}
float get_fp(int val) {
	float tworaisedE=1.0;
	//if input is zero return 0
	if (val ==0){
		return 0.0;
	}
	// get a floating point struct
	Fp newfp =get_struct_fp(val);
	//get exp from new fp struct and substract bias
	int E= newfp.exp - 7;
	// check for infinity
	if (E>7 && newfp.frac==0){
		if (newfp.sign ==1){
			return INFINITY;
		}
		else{
			return -INFINITY;
		}
	}
	// check for pos and neg zero 
	if (newfp.exp<=0 &&newfp.frac == 0){
		if (newfp.sign ==1){
			return 0;
		}
		else{
			return -0;
		}
	}
	//compute 2^E based on if E is positive or negative
	while (E>0){
		tworaisedE *=2.0;
		E--;
	}
	while (E<0){
		tworaisedE /=2.0;
		E++;
	}
	//obtain manitssa and compute the return value
	float mantissa = 1+(newfp.frac/64.0);
	float mynum= (newfp.sign *mantissa * tworaisedE);
  return mynum;
	
}

/* input: Two integer representations of our floats
 * output: The multiplication result in our integer representation
 *
 * You must implement this using the algorithm described in class:
 *   Xor the signs: S = S1 ^ S2
 *   Add the exponents: E = E1 + E2
 *   Multiply the Frac Values: M = M1 * M2
 *   If M is not in a valid range for normalized, adjust M and E.
 *
 * Follow the Project Documentation for Instructions
 */
int mult_vals(int source1, int source2) {
	int returnval,sign,E,frac,sign1, sign2,exp;
	float mantissa;
	// get the fp structs and correct signs 
	Fp fp1= get_struct_fp(source1);
	Fp fp2=get_struct_fp(source2);
	sign1 =(source1& 0x400)>>10;
	sign2= (source2& 0x400)>>10;
	// sign is xor of signs E is E1 + E2
	sign = sign1^sign2;
	//Check if zero or Infinity at beginning 
	if (fp1.exp<=0 || fp1.exp>14){
		Fp myfp=check_fp(fp1.exp,fp1.sign,fp1.frac);
		//pack int and return
		returnval= packint(sign,fp1.exp,fp1.frac);
		return returnval;
	}
	if (fp2.exp<=0||fp2.exp>14 ){
		Fp myfp=check_fp(fp2.exp,fp2.sign,fp2.frac);
		//pack int and return
		returnval= packint(sign,fp2.exp,fp2.frac);
		return returnval;
	}
	E = (fp1.exp-7) + (fp2.exp-7);
	exp = E+7;
	mantissa= ((fp1.frac/64.0)+1.0);
	mantissa*=((fp2.frac/64.0)+1.0);
	//adjust mantissa if needed
	if (mantissa>=2){
		while(mantissa>=2){
		mantissa/=2;
		E++;
		}
	}
	else{
		while (mantissa<1){
		mantissa*=2;
		E--;
		}
	}
	//get frac from mantissa and round obtain exp using bias
	mantissa= mantissa-1;
	mantissa=mantissa *64;
	frac=round_frac(mantissa);
	exp= E+7;
	//check underflow &over flow
	Fp myfp=check_fp(exp,sign,frac);
	sign= myfp.sign;
	exp= myfp.exp;
	frac= myfp.frac;
	//pack int and return
	returnval= packint(sign,exp,frac);
  return returnval;
}

/* input: Two integer representations of our floats
 * output: The addition result in our integer representation
 *
 * You must implement this using the algorithm described in class:
 *   If needed, adjust the numbers to get the same exponent E
 *   Add the two adjusted Mantissas: M = M1 + M2
 *   Adjust M and E so that it is in the correct range for Normalized
 *
 * Follow the Project Documentation for Instructions
 */
int changesigns(Fp fp1){
	//right now sign is stored as -1 or 1 
	//change to 1 or 0 respectivly
	if(fp1.sign ==1){
		return 0;
	}
	return 1;
}
int add_vals(int source1, int source2) {
  /* Implement this function */
	int E1,E2,exp,frac,returnval;
	Fp temp;
	float mantissa;
	//get two  floating point structs
	Fp fp1= get_struct_fp(source1);
	Fp fp2=get_struct_fp(source2);
	// check for case of  INFINITY
	if ( fp1.exp>14){
		Fp myfp=check_fp(fp1.exp,fp1.sign,fp1.frac);
		//pack int and return
		int sign1=changesigns(fp1);
		returnval= packint(sign1,fp1.exp,fp1.frac);
		return returnval;
	}
	if (fp2.exp>14 ){
		Fp myfp=check_fp(fp2.exp,fp2.sign,fp2.frac);
		//pack int and return
		int sign1=changesigns(fp2);
		returnval= packint(sign1,fp2.exp,fp2.frac);
		return returnval;
	}
	//make sure E1 is greater than E2 otherwise switch
	if ( fp1.exp < fp2.exp ){
		temp=fp1;
		fp1=fp2;
		fp2=temp;
	}
	//put sign into zero or 1
	int sign1=changesigns(fp1);
	// get E from exp by subtracting bias
	E1= fp1.exp-7;
	E2 = fp2.exp-7;
	//obtain first mantissa
	mantissa=((fp2.frac/64.0)+1.0);
	//adjust exponants if needed
	if (E1!= E2){
		// adjust mantissa and E accordingly
		if (E1>E2){
		while (E1> E2){
			mantissa/=2;
			E2++;
		}
		}
		else if (E2>E1){
			while (E2> E1){
			mantissa*=2;
			E2--;
		}
		}
		
	}
	//either add or subtract mantissa based on sign 
	if((fp1.sign == fp2.sign)){
		mantissa+=((fp1.frac/64.0)+1.0);
	}
	else{
		mantissa-=((fp1.frac/64.0)+1.0);
		if (mantissa<0){
			mantissa*=-1;
		}
	}
	//re adjust our new mantissa if needed
	if (mantissa>=2){
		while(mantissa>=2){
		mantissa/=2;
		E1++;
		}
	}
	else{
		while (mantissa<1){
		mantissa*=2;
		E1--;
		}
	}
	// extract and round fract and get exp using bias
	mantissa= mantissa-1;
	mantissa=mantissa *64;
	frac=round_frac(mantissa);
	exp = E1+7;
	//again check over and underflow 
	Fp myfp=check_fp(exp,sign1,frac);
	sign1= myfp.sign;
	exp= myfp.exp;
	frac= myfp.frac;
	//pack int  and return 
	returnval= packint(sign1,exp,frac);
  return returnval;
}
