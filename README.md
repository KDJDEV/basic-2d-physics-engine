# Physics Engine

A basic 2d physics engine that supports elastic collisions between circles and rectangles, springs, pendulums, gravity, graphing of 2d phase spaces, and whatever other random features I wanted to create. Though not very robust, it can still be used to simulate some fun stuff. Its main problem is that it uses the Euler method of integration, which is not very accurate. In the future I modify this engine to use a more robust approach by switching to a more constraint based model and using a better differential equation solver like Runge-Kutta(such as in this [simple-2d-constraint-solver](https://github.com/ange-yaghi/simple-2d-constraint-solver)).

I made this program in Visual Studio 2022 using C++20. I included the .sln project file here in the source, so if you clone this repository you should be able to open that file in VS to get things working right away.

You can find some prebuilt simulations you can try out in miskTests.cpp. To try them out, switch out the default test below the "switch out the miskTest here" comment in main.cpp. Then, run the program. You can also change some basic global settings in config.cpp.
