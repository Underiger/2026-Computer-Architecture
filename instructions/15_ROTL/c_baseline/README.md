# 15_ROTL/c_baseline/

- 對應 Checkpoint:CP1
- 內容:C 參考實作(原始碼、與 spec 一致的參考函式)

| 檔案 | 內容 |
|---|---|
| `rotl.h` / `rotl.c` | C 參考實作 `uint32_t rotl_ref(uint32_t rs1, uint32_t rs2)`,與規格逐字對應 |
| `test_rotl.c` | 主機測試:CSV 測資、與逐位旋轉寫法交叉比對(69,000 次)、性質檢查 |
| `ub_demo.c` | 反例:未處理 `n = 0` 的寫法,在 UBSan 下會報出移位 32 的未定義行為 |

```bash
cd instructions/15_ROTL/c_baseline
make test       # test_rotl + UBSan
make ub-demo    # 反例(n = 0 時預期報錯)
```

設計與測試結果見 [`../report/CP1.md`](../report/CP1.md)。
