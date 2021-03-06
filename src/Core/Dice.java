/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Core;

import java.util.Random;
/**
 *
 * @author Impaler
 */
public class Dice {
    
    public Random Generator;
    
	public Dice() {
        Generator = new Random();
        Seed(0);		
	}

    public final void Seed(long Seed) {
        Generator.setSeed(Seed);
    }

    public int Roll(int Min, int Max) {
        return Generator.nextInt(Max - Min + 1) + Min;
    }

    public float Roll(float Min, float Max) {
        return Generator.nextFloat() * (Max - Min) + Min;
    }
}
