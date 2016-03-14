module app.d.toolbox.tc;

import std.stdio;
import std.conv;

import cgem.d.physics.temperature;

int tc(string[] args)
{
    double temperatureF = to!double(args[1]);

    Temperature t;
    t.F(temperatureF);
   
    auto freezingCold = t.C < 0;// water freezes at 0 degrees Celsius.
	
    // Tell user result of the convertion.
    writefln("Temperature %.2f °F = %.2f °C, freezing? %s",
      t.F, t.C, freezingCold);
	
    // Print small convertion table around given temperature.
    foreach (offset; -3..4) {
      t.F(temperatureF + offset); //t.value += offset;
      writefln("%.2f °F : %.2f °C", t.F, t.C);
    }

    return 0;
}

