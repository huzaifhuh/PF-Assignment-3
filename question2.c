#include <stdio.h>

void calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets) {

    if (fuel <= 0)
    {
        printf("Planet %d: Fuel Remaining = 0\n", planet - 1);
        printf(">>> MISSION FAILED! Ran out of fuel before completing journey!\n");
        return;
    }

    if (planet > totalPlanets)
    {
        printf("Planet %d: Fuel Remaining = %d\n", totalPlanets, fuel);
        printf(">>> MISSION SUCCESS! Spacecraft reached home safely!\n");
        return;
    }

    printf("Planet %d: Fuel Remaining = %d", planet, fuel);

    fuel = fuel - consumption + recharge;

    if (planet % 4 == 0)
    {
        fuel = fuel + solarBonus;
        printf("  --> SOLAR RECHARGE! +%d fuel", solarBonus);
    }

    printf("\n");

    calculateFuel(fuel, consumption, recharge, solarBonus, planet + 1, totalPlanets);
}

int main()
{
    int startingFuel, consumption, recharge, solarBonus, totalPlanets;

    printf("=== SPACECRAFT FUEL SIMULATION ===\n\n");

    printf("Enter starting fuel: ");
    scanf("%d", &startingFuel);

    printf("Enter fuel consumed per planet: ");
    scanf("%d", &consumption);

    printf("Enter normal fuel recharge per planet: ");
    scanf("%d", &recharge);

    printf("Enter solar bonus (every 4th planet): ");
    scanf("%d", &solarBonus);

    printf("Enter total number of planets: ");
    scanf("%d", &totalPlanets);

    printf("\n--- Journey Started ---\n");

    calculateFuel(startingFuel, consumption, recharge, solarBonus, 1, totalPlanets);

    printf("\nSimulation ended.\n");
    return 0;
}