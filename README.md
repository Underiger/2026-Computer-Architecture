# Computer Architecture 2026 — Custom RISC-V Instruction 學期專題

本倉庫為 2026 學年度第一學期《計算機組織》課程的學期專題資料,主軸為:每組選擇一條 **Custom RISC-V Instruction**,完成從 C baseline 到 5 級 Pipeline 的完整分析。

> 核心問題:如果 RISC-V CPU 要加入這條新指令,CPU 裡面需要增加或修改什麼?

## 專題流程

```
C baseline → 標準 RISC-V 實作 → Custom Instruction 規格與 Encoding   (期中前)
           → Datapath → Control → 5 級 Pipeline 說明                  (期中後)
           → Hazard / Forwarding → Performance 比較 → 期末整合展示
```

## 分組與題目

- 修課人數:54 人
- 分組方式:**11 組 4 人 + 2 組 5 人,共 13 組**,每組對應一個題目,不重複
- 選題截止:**Week 5(2026/10/06)當週結束前**,由選題表單完成

### 學生可選題目(19 題)

| 類別 | 題目 |
|---|---|
| 基礎運算類(7 題) | `ABS`、`MIN`、`AVG`、`CLAMP`、`ABSDIFF`、`THRESHOLD`、`MAXPIX` |
| AI / 電腦視覺類(5 題) | `ReLU6`、`Leaky ReLU`、`CLIP`、`POPCOUNT`、`SAD` |
| 網路類(2 題) | `NETMASK`(子網路遮罩套用)、`CSUM16`(IP checksum 一補數加法) |
| 密碼學 / 雜湊類(1 題) | `ROTL`(循環左移) |
| DSP 類(2 題) | `ADDSAT`(飽和加法)、`BITREV`(位元反轉) |
| 字串 / 資料處理類(1 題) | `FINDZERO`(找 32-bit 字中的 0 byte) |
| 算術類(1 題) | `CLZ`(前導零計數) |

### 示範題(不開放選題)

| 題目 | 負責 | 用途 |
|---|---|---|
| `MAC`(Multiply-Accumulate) | 教師 / 助教 | Week 7 課堂示範,展示乘法器與加法器如何組成客製化指令 |
| `TOP2_4`(4×INT8 找最大與第二大) | 進階學長 | 展示 packed data 與 Comparator Network 的設計深度 |

## 統一指令格式

所有組別使用 R-type 格式,`opcode`、`funct3`、`funct7` 由教師統一配置,避免衝突並利於期末比較:

```
31        25 24    20 19    15 14  12 11     7 6       0
+-----------+--------+--------+------+---------+---------+
|  funct7   |  rs2   |  rs1   |funct3|   rd    | opcode  |
+-----------+--------+--------+------+---------+---------+
```

各組需定義:`rs1` 與 `rs2` 的意義、`rd` 的回傳內容、指令語意。

## 組內分工參考

| 組員 | 初步負責範圍 |
|---|---|
| A | C / 標準 RISC-V Baseline |
| B | Instruction Encoding / ISA 規格 |
| C | Datapath / Control |
| D | Pipeline / Hazard / Forwarding |
| E(僅 5 人組) | Performance 分析 / 簡報整合 |

## 行事曆與 Checkpoint

| 週次 / 日期 | 課程主題 | 專題 Checkpoint |
|---|---|---|
| W5 · 10/06 | Instruction Format / Machine Code | HW2 發布、選題確認 |
| W6 · 10/13 | Compiler / Assembler 流程 | Checkpoint 1+2:C baseline、標準 RISC-V 實作 |
| W7 · 10/20 | Computer Arithmetic、MAC Demo | 持續實作、示範題觀摩 |
| W8 · 10/27 (停課,自主學習) | 期中整合 | **繳交書面報告(CP3)**:指令規格、編碼、instruction count 比較(期中前版本),不安排上台 |
| W9 · 11/03 | **期中考**(涵蓋 W1–8) | — |
| W10 · 11/10 | Processor Datapath | Checkpoint 4 開始 |
| W11 · 11/17 | Single-Cycle Processor 與 Control | 分析 control signal 需求 |
| W12 · 11/24 (停課,小組討論) | Datapath 整合 | **繳交 Datapath 圖與說明** |
| W13 · 12/01 | Pipeline 基礎(5 級 IF/ID/EX/MEM/WB) | Checkpoint 5:指令放入 5 級 Pipeline |
| W14 · 12/08 | Pipeline Hazard | Checkpoint 6 開始:判斷 hazard |
| W15 · 12/15 | Forwarding / Stall / Bubble | **繳交 Pipeline timing 與 Hazard 分析** |
| W16 · 12/22 | 整合:ISA → Pipeline | **期末成果展示 / Demo**(隨機抽問,組內皆須理解) |
| W17 · 12/29 | 全學期回顧 | — |
| W18 · 01/05 | 期末考 | 筆試,不含專題展示 |

另有 10/07、10/14、10/21 晚上 18:00–19:00 指令集加課,作為 HW2 實作工作坊。

## Pipeline 說明模板(期中後)

每組需在 5 級 pipeline 中標示自己指令在各 stage 的工作:

```
 IF        ID          EX          MEM       WB
 │         │           │           │         │
Fetch   Decode     自訂指令執行    (是否使用)  Writeback
        (讀 rs1/rs2)  在這裡嗎?
```

必須回答:

1. 原本 ALU 能否在 EX 一次完成?
2. 若運算複雜(如 Comparator / Adder Tree),是否需拆成多個 stage,對 Clock Cycle Time 的影響為何?
3. 是否使用 MEM stage?
4. `rd` 在哪個 stage 寫回 Register File?
5. Control Unit 需新增哪些控制訊號?
6. 指令的 combinational path 是否會成為全 CPU 的 Critical Path?

## 評估重點

以 **Instruction Count × CPI × Clock Cycle Time** 為主要量化框架,比較 Standard RV32IM 與 Custom ISA。不要求真實硬體 benchmark,重點在於看懂並說明。

---

## 繳交與聯絡

- 作業與公告以課程平台為準
- 選題與分組問題請於 Week 5 前洽授課教師或助教
