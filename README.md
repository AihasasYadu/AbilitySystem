# AbilitySystem
This is a Unreal Engine Gameplay Ability System (GAS) implementation for an action game featuring a leap slam melee combat mechanic. The codebase provides a solid foundation with character classes, attributes, and one complete ability implementation.
The Leap Slam allows the player to leap over a certain distance and obstacles and then slams down with a force damaging surrounding enemies in the attack radius. I've also added an additional attack area to attack slightly forward as well, in the forward direction of the character.

GAS Foundation Layer:-
1. UAttributeSetBase: Core attributes (Health/MaxHealth, Mana/MaxMana, AttackSpeed/MaxAttackSpeed) with replication and clamping
2. UGameAbilitySystemComponent: Minimal extension of UE's ASC with ticking enabled
3. ACharacterBase: Base character implementing IAbilitySystemInterface, auto-initializes ASC + AttributeSet + default abilities

Player Characters:-
1. AMainCharacter: Player pawn with top-down camera, input mapping (Move + Ability), LeapSlam activation
2. AGamePlayerState: PlayerState with GAS support for replicated attributes

Enemy:-
1. AEnemyCharacter: Basic enemy inheriting CharacterBase, overrides TakeDamage to display damage taken


Leap Slam Ability System:-
1. ULeapSlam: Core GameplayAbility implementing leap → land → damage sequence (Displays Debug Spheres to display Attack Impact)
2. ULeafArc: Custom AbilityTask for parabolic character launch based on input direction
3. UWaitCharacterLanded: Custom AbilityTask waiting for character landing detection


UAttributeSetBase (6 attributes, fully replicated) - Standard GAS attribute container with replication, clamping (Health can't go below 0 or above MaxHealth), and Blueprint exposure. Uses GAS macros for getter/setter generation.
1. Health (clamped 0-MaxHealth, OnRep notifications)
2. MaxHealth
3. Mana/MaxMana (Cost for using an Ability)
4. AttackSpeed/MaxAttackSpeed (Defines the strength and scale of attack)


UGameAbilitySystemComponent - Blueprint-spawnable ASC extension. Minimal - just enables ticking and replication. Ready for future ability grants/effects.


ACharacterBase : (ACharacter + IAbilitySystemInterface) - Every GAS-enabled character inherits this. Handles component creation, ability granting, and attribute initialization automatically.
1. Components: UGameAbilitySystemComponent + UAttributeSetBase
2. BeginPlay(): Creates components, grants default abilities, applies attribute effects
3. GetAbilitySystemComponent() → returns ASC
4. GetAttributeSet() → returns AttributeSetBase


AMainCharacter : ACharacterBase - Playable character with WASD movement + ability input. LeapSlam activates on UseAbilityAction.
1. Top-down camera (SpringArm + CameraComponent)
2. Enhanced Input: MoveAction + UseAbilityAction
3. LeapSlamAbilityTag (FGameplayTag) for input activation
4. Input callbacks: Move() + UseAbility() → TryActivateAbilityByTag


Ability Flow:-

Pre Leap Delay → Launch character (input-directed parabolic arc) → Hit Delay After Launch → Lands On Ground → Perform Damage inside Radius → Recovery Delay After Hit → End

Ability Blueprints:-
1. GA_LeapAndSlam - Main Gameplay Ability BP for LeapSlam Attack
2. GE_LeapSlamCost - Addresses the Cost Attribute for LeapSlam Attack (Mana)
3. GE_LeapSlamCooldown - Sets a cooldown for ability (currently set to 1 sec)
