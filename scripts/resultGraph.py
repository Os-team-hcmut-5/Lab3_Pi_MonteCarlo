import matplotlib.pyplot as plt

# Data from results
threads = [1, 2, 4, 8, 16, 32, 64, 100]
thread_labels = [str(t) for t in threads] # Use strings for equal spacing
x_pos = range(len(threads)) # 0, 1, 2, 3, 4, 5, 6, 7

# Approach 2 (Local)
times_ap2 = [2.723754, 1.468432, 0.839503, 0.540143, 0.408749, 0.293819, 0.317002, 0.281876]
speedup_ap2 = [1.00, 1.85, 3.24, 5.04, 6.66, 9.27, 8.59, 9.66]

# Approach 3 (Shared Mutex)
times_ap3 = [3.525387, 4.775134, 5.643507, 6.328122, 5.210236, 5.275478, 5.361644, 5.361862]
speedup_ap3 = [1.00, 0.74, 0.62, 0.56, 0.68, 0.67, 0.66, 0.66]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))
fig.suptitle('Performance Comparison: Local Counters vs. Mutex Contention', fontsize=16, fontweight='bold')

# Left Plot: Execution Time (using x_pos for equal spacing)
ax1.plot(x_pos, times_ap2, 'bo-', label='Approach 2 (Local)', linewidth=2)
ax1.plot(x_pos, times_ap3, 'rs--', label='Approach 3 (Shared Mutex)', linewidth=2)
ax1.set_xticks(x_pos)
ax1.set_xticklabels(thread_labels)
ax1.set_xlabel('Number of Threads', fontsize=12)
ax1.set_ylabel('Execution Time (seconds)', fontsize=12)
ax1.set_title('Execution Time vs. Number of Threads', fontsize=14)
ax1.grid(True, linestyle=':', alpha=0.6)
ax1.legend()

ax2.plot(x_pos, speedup_ap2, 'bo-', label='Approach 2 (Local)', linewidth=2)
ax2.plot(x_pos, speedup_ap3, 'rs--', label='Approach 3 (Shared Mutex)', linewidth=2)
ax2.axhline(y=1.0, color='gray', linestyle='-.', alpha=0.5, label='Baseline (1 Thread)')
ax2.set_xticks(x_pos)
ax2.set_xticklabels(thread_labels)
ax2.set_xlabel('Number of Threads', fontsize=12)
ax2.set_ylabel('Speedup Multiplier ($T_{single} / T_{parallel}$)', fontsize=12)
ax2.set_title('Speedup vs. Number of Threads', fontsize=14)
ax2.grid(True, linestyle=':', alpha=0.6)
ax2.legend()

plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.savefig('../results/performance_comparison.png', dpi=300)
print("Saved to ../results/performance_comparison.png")