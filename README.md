rpi-networking
==============
g++-4.8 -std=c++11 -I ../../boost_1_55_0 -I ../ -I ../gpio/ -I ../wiring_pi/ -L ../../boost_1_55_0/stage/lib -pthread ../atan/actor/actor.cpp ../atan/actor/remote_actor.cpp server.cpp -o a.out ../../boost_1_55_0/stage/lib/libboost_serialization.a ../../boost_1_55_0/stage/lib/libboost_system.a
