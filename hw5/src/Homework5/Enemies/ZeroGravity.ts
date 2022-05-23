import Vec2 from "../../Wolfie2D/DataTypes/Vec2";
import GameEvent from "../../Wolfie2D/Events/GameEvent";
import AnimatedSprite from "../../Wolfie2D/Nodes/Sprites/AnimatedSprite";
import { HW5_Events } from "../hw5_enums";
import BalloonState from "./BalloonState";

// HOMEWORK 5
/**
 * For this homework, you'll have to implement an additional state to the AI from scratch.
 * 
 * This new behavior should be for the zero gravity balloon state, where the balloon no
 * longer has gravity affecting it.
 * 
 * Along with this, the balloon should move twice it's current velocity if it's close
 * to the player, within about 10 tiles. You only have to apply this speed change to the
 * x velocity, the y velocity will be left unchanged.
 * 
 * When the player moves far enough away again, the balloon should return to it's original velocity.
 * 
 * You can implement this method how you see fit, there's no one way of doing it. Look at events that
 * are fired to get the player position
 */
export default class ZeroGravity extends BalloonState {
	onEnter(): void {
		this.gravity = 0;
		(<AnimatedSprite>this.owner).animation.play("IDLE", true);
	}
	update(deltaT: number): void {
		super.update(deltaT);
		this.parent.velocity.x = this.parent.direction.x * this.parent.speed;
		this.owner.move(this.parent.velocity.scaled(deltaT));
	}

	handleInput(event: GameEvent): void {
		super.handleInput(event);
		if (event.type === HW5_Events.PLAYER_MOVE) {
			if (this.parent.owner.position.distanceTo(event.data.get("position")) <= 320) {
				this.parent.speed = 200;
			}
			else {
				this.parent.speed = 100;
			}
		}
	}

	onExit(): Record<string, any> {
		(<AnimatedSprite>this.owner).animation.stop();
		return {};
	}
}