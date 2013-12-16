rpi-networking
==============
g++-4.7 -std=c++11 -I../../boost_1_55_0 -I ../ -I ../gpio/ -L ../../boost_1_55_0/stage/lib -pthread ../atan/actor/actor.cpp ../atan/actor/remote_actor.cpp server.cpp -o a.out ../../boost_1_55_0/stage/lib/libboost_serialization.a ../../boost_1_55_0/stage/lib/libboost_system.a -D_GLIBCXX_USE_NANOSLEEP -lwiringPi -O2
