#ifndef __DHARMA_H
#define __DHARMA_H




typedef enum tagCOHACK_APTTYPE
{
    COHACK_APTTYPE_TNA        = 0,
    COHACK_APTTYPE_STA        = 1,
    COHACK_APTTYPE_MTA        = 2
} COHACK_APTTYPE;

COHACK_APTTYPE CoHackGetAptType();


char* GetApartmentType();

//Thanks to Kevin Jones of DevelopMentor for giving the offsets in the TEB, 
//where the apartment types are stored, on the ATL listserver. He said he had tested them for STA and MTA on NT4.
//I experimented with these magic numbers<g> and added the stuff for the Thread Neutral Apartment 
//on NT5 and also verified Kevin's offsets for NT5beta.
//Apparently the value stored in TLS index/returned by CoHackGetTLSValue(), is a bunch of ORed 
//flags which keep on changing depending on if the object is created directly 
//in the creator's apartment or a COM created apartment; on whether the 
//object is inproc or out-of-proc etc.etc. CoHackGetAptType() looks for the magic number 
//out of the bunch of ORed flags and seems to work reliably in all scenarios,
//so far:-)

#endif