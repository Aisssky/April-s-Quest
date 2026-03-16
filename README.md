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

  - 基于 AnimBlueprint 搭建角色状态机，管理 Idle / Run /
  - Jump / Attack / Death / Vault /Run /Crouch 完整状态转换；使用 BlendSpace
  - 处理方向移动与速度的动画混合；AnimMontage 实现攻击
  - 连击系统，AnimNotify 在关键帧触发伤害判定与音效播放。 

---
