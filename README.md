# UE5 ARPG Demo
 
基于 UE5 开发的单机 ARPG，从蓝图原型出发，使用 C++ 完整重构核心系统。
预计涵盖 GAS 战斗、背包、剧情对话、技能树，以及 UI 管理框架。

 
## 开发环境
 
UE 5.6 · Visual Studio 2022 · Windows

---
 
## 核心模块
 
| 模块 | 实现方式 |
|------|---------|
| 战斗系统 | C++ + GAS（AttributeSet / GameplayAbility / GameplayEffect） |
| UI 框架 | GameInstanceSubsystem + BasePanel + 栈式管理 |
| 对话系统 | DataAsset 数据驱动 + DialogueComponent |
| 背包系统 | InventoryComponent + 委托解耦 UI |
| 存档系统 | USaveGame + 自定义序列化（含剧情进度） |
| AI 系统 | 行为树 + C++ BTTask / BTService |
| 蓝图 | 仅用于动画、特效、参数配置，不写业务逻辑 |
 
---

## 日志

动画系统：

  - 基于 AnimBlueprint 搭建角色状态机，管理 Idle / Run / Jump / Attack / Death / Vault /Run /Crouch 完整状态转换；
  - 使用 BlendSpace处理方向移动与速度的动画混合；
  - AnimMontage 实现攻击连击系统，AnimNotify 在关键帧触发伤害判定与音效播放。 

---
- 搭建 AAQCharacterBase 作为所有角色的 C++ 基类
- 接入 GAS 框架：ASC + AttributeSet + 初始属性 GE
- 实现 AQGameplayAbility 技能基类和 GA_Attack 攻击技能
- 实现 AQCombatComponent 球形检测 + GE 伤害施加
- 蓝图 ANS_MeleeAttack 驱动攻击窗口开关
- 打通攻击：按键→GAS激活→Montage→AnimNotify→伤害判定
- 规范项目目录结构，删去不必要的文件
---

- 向UAQAttributeSet添加FOnAttributeChanged委托
- 在PostGameplayEffectExecute中广播Health/Stamina的变化
- 实现带有ProgressBar/TextBlock BindWidget的UAQHUDPanel
- 在AQPlayerCharacter::BeginPlay中连接UIManager的OpenPanel调用

- 数据流：
  玩家攻击 → HandleAttack()
    → TryActivateAbilityByTag
    → UAQAbility_Attack::ActivateAbility → PlayAnimMontage
    → ANS 通知 → CombatComponent::EnableAttack() 这段还没做
    → TickComponent → PerformAttackTrace → 命中目标
    → ApplyGameplayEffectSpecToTarget (DamageEffect)
    → UAQAttributeSet::PostGameplayEffectExecute
        → SetHealth(NewHealth)
        → OnHealthChanged.Broadcast(NewHealth, MaxHealth)
    → UAQHUDPanel::OnHealthChanged(NewValue, MaxValue)
        → PB_Health->SetPercent(Percent)

  ---
  
- 修复 GE_DefaultAttributes 中的修饰器顺序（先最大值后当前值）【否则Health 初始值被 Clamp 成 0
- 添加继承自 AAQCharacterBase 的 AAQEnemyCharacter，并启用 AutoPossessAI
- 添加 AAQAIController，并在 OnPossess 中启动行为树
- 在 Build.cs 中添加 AIModule 和 NavigationSystem 模块依赖
- 骨骼资产污染问题
  
   不同来源的骨骼网格体不能共用同一个骨骼资产。导入外部角色时必须给它创建独立的骨骼，否则会污染已有骨骼。还好接了版本控制【喜，下次导入新骨骼网格体时，Skeleton 一栏必须选 None，让 UE 创建独立骨骼，或者选择新建项目然后做 做好了再迁移资产
---

- 新增继承自AAQCharacterBase的AAQEnemyCharacterBase
- 新增带有AIPerceptionComponent（视觉配置）的AAQAIController
- 实现OnTargetPerceptionUpdated用于更新黑板
- BT_Enemy：包含追逐序列和空闲等待的选择器
- 在丢失视野后保留TargetActor以实现持续追击"

---
