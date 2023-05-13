This is key mismatch attack demo for Kyber512 in NIST Round 3 
and $sk \in [-3,3], (d_u,d_v) = (10,4)$.

# Structure

PQCgenKAT_kem.c: the entrance of attack, 

kem.c:  building the oracle 

indcpa.c: choosing attack parameters


# Build and Run

To build it, you need to have openssl  and make on linux or Mac os.

> make

After making, then you can run 

>  ./PQCgenKAT_kem \<num\>

`<num>` is a integer used as a random seed. For example, `./PQCgenKAT_kem 1`
