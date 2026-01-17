**Simulation Overview**

This repository presents a simulation of digital communication over an additive white Gaussian noise (AWGN) channel, representative of a 5G wireless communication environment.

The system models the end-to-end transmission of encoded code blocks using Low-Density Parity-Check (LDPC) coding and Quadrature Phase Shift Keying (QPSK) modulation.

System Parameters

Code rate: 0.43

Block length: 308 bits

Modulation scheme: QPSK

Channel model: Gaussian (AWGN)

Coding Structure

G: Generator matrix used at the transmitter for LDPC encoding.

H.txt: Parity-check (LDPC) matrix used at the receiver for decoding.

The simulation evaluates the performance of the coded communication system under Gaussian noise conditions, consistent with physical-layer assumptions in contemporary 5G systems.
