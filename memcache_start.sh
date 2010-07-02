#!/bin/sh

memcached -d -m 32 -p 36841 -l 127.0.0.1
memcached -d -m 32 -p 36842 -l 127.0.0.1
