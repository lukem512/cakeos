# Introduction #

http://bochs.sourceforge.net/techspec/intel-mp-spec.pdf.gz (Intel MP Specification)

# MP Table Configuration/Detection (Sample Code) #

//This will detect the mp table, and fill in the data struct
void detect\_mp()
{
> typedef struct mptable
> {
> > unsigned int magic;
> > unsigned int physical;
> > unsigned char length;
> > unsigned char version;
> > unsigned char check;
> > unsigned char features[4](4.md);

> } MPTable\_t;

> MPTable\_t**mp;**

> //Extended BIOS data area (EBDA)
> unsigned int **ptr = (unsigned int**)0x0000040E;
> while(ptr < 0x0000440E)
> {
> > if(**ptr ==**"_MP_")
> > {


> mp = (MPTable\_t**)ptr;**

> if(mp->length != 0)
> > if((mp->magic + mp->physical
> > > + mp->length + mp->version
> > > + mp->check + mp->features[0](0.md)
> > > + mp->features[1](1.md) + mp->features[2](2.md)
> > > + mp->features[3](3.md) + mp->features[4](4.md)) == 0){
> > > kprintf("Zoop!\n");
> > > break;}

> }
> ptr+=16;
> }

> //BIOS ROM
> ptr = (unsigned int**)0xF0000;
> while(ptr < 0xFFFFF)
> {
> > if(**ptr == **"_MP_")
> > {**


> mp = (MPTable\_t**)ptr;**

> if(mp->length != 0)
> > if((mp->magic + mp->physical
> > > + mp->length + mp->version
> > > + mp->check + mp->features[0](0.md)
> > > + mp->features[1](1.md) + mp->features[2](2.md)
> > > + mp->features[3](3.md) + mp->features[4](4.md)) == 0){
> > > kprintf("Zoop!\n");
> > > break;}

> }
> ptr+=sizeof(MPTable\_t);
> }
}