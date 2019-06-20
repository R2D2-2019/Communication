#include <communication.hpp>


<template T>
communication_c::receive_data_to_send(){
	frame_s frame = comm.get_data();
	reinterpret_cast<const uint8_t *>(*frame);
	sizeof(frame);
}