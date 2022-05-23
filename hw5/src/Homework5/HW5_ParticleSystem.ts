import Vec2 from "../Wolfie2D/DataTypes/Vec2";
import Particle from "../Wolfie2D/Nodes/Graphics/Particle";
import ParticleSystem from "../Wolfie2D/Rendering/Animations/ParticleSystem";
import Color from "../Wolfie2D/Utils/Color";
import { EaseFunctionType } from "../Wolfie2D/Utils/EaseFunctions";
import MathUtils from "../Wolfie2D/Utils/MathUtils";
import RandUtils from "../Wolfie2D/Utils/RandUtils";

// HOMEWORK 5
/**
 * This particle system extends the base ParticleSystem class, and I reccommend you look at some of the implementation, 
 * at least for the default setParticleAnimation()
 * 
 * You'll just be handling the tweens for each particle for their animation, overriding the base implementation.
 * 
 * The new particle animation add these behaviors, along with the existing setParticleAnimation behaviors:
 * 
 *  - Each particle should look like they're affected by gravity, accelerating down over the course of their lifetime. This
 *  change should also be affected by the particle's mass, meaning particles with a higher mass should fall faster.
 * 
 *  - Each particle should disappear over it's lifetime, moving from an alpha of 1 to 0.
 */
export default class HW5_ParticleSystem extends ParticleSystem {
    startSystem(time: number, mass?: number, startPoint?: Vec2): void {
        super.startSystem(time, mass, startPoint);
        switch (mass) {
            case 1:
                super.changeColor(Color.RED);
                break;
            case 2:
                super.changeColor(Color.GREEN);
                break;
            case 3:
                super.changeColor(Color.BLUE);
                break;
            default:
                break;
        }
    }

    setParticleAnimation(particle: Particle) {
        super.setParticleAnimation(particle);
    }
        update(deltaT: number) {
        // Exit if the system isn't currently running
        if (!this.systemRunning) {
            return;
        }
        // Stop the system if our timer is up
        if (this.systemLifetime.isStopped()) {
            this.stopSystem();
        }
        else {
            for (let i = 0; i < this.particlesToRender; i++) {
                let particle = this.particlePool[i];

                // If a particle is in use, decrease it's age and update it's velocity, if it has one
                if (particle.inUse) {
                    particle.decrementAge(deltaT * 1000);
                    particle.alpha = particle.age/2000;
                    particle.vel = particle.vel.add(new Vec2(0, 3*particle.mass));
                    if (particle.age <= 0) {
                        particle.setParticleInactive();
                    }

                    particle.move(particle.vel.scaled(deltaT));
                }
                else {
                    // Set the particle to active
                    particle.setParticleActive(this.lifetime, this.sourcePoint.clone());

                    // Update particle color, mass, and alpha
                    particle.color = this.color;
                    particle.alpha = 1;
                    particle.mass = this.particleMass;
                    
                    // Give particle tween animations
                    this.setParticleAnimation(particle);

                    particle.tweens.play("active");
                }
            }
            // Update the amount of particles that can be rendered based on the particles per frame, clamping if we go over the total number
            // of particles in our pool
            this.particlesToRender = MathUtils.clamp(this.particlesToRender+this.particlesPerFrame, 0, this.particlePool.length);
        }
    }
}