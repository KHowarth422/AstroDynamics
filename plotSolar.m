sun = readtable('Sun.txt');
mercury = readtable('Mercury.txt');
venus = readtable('Venus.txt');
earth = readtable('Earth.txt');
mars = readtable('Mars.txt');
jupiter = readtable('Jupiter.txt');
saturn = readtable('Saturn.txt');
uranus = readtable('Uranus.txt');
neptune = readtable('Neptune.txt');

%% Plotting

plot(sun{:,'Pos_X'},sun{:,'Pos_Y'},'h')
hold on
plot(mercury{:,'Pos_X'},mercury{:,'Pos_Y'})
plot(venus{:,'Pos_X'},venus{:,'Pos_Y'})
plot(earth{:,'Pos_X'},earth{:,'Pos_Y'})
plot(mars{:,'Pos_X'},mars{:,'Pos_Y'})
plot(jupiter{:,'Pos_X'},jupiter{:,'Pos_Y'})
plot(saturn{:,'Pos_X'},saturn{:,'Pos_Y'})
plot(uranus{:,'Pos_X'},uranus{:,'Pos_Y'})
plot(neptune{:,'Pos_X'},neptune{:,'Pos_Y'})

title('Solar System')
xlabel('X Position (m)')
ylabel('Y Position (m)')
legend('Sun','Mercury','Venus','Earth','Mars','Jupiter','Saturn','Uranus','Neptune')

hold off
