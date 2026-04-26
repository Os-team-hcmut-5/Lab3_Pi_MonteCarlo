import matplotlib.pyplot as plt

# Data from Thread Scaling Experiment
threads = [1, 2, 4, 8, 12, 16, 20, 24, 32, 48, 64, 128, 256, 512, 1024]
# Because the x-axis values vary drastically (from 1 to 1024), we treat them as categories
# so the graph doesn't cramp all the small numbers to the left side.
thread_labels = [str(t) for t in threads]
x_pos = range(len(threads))

time_ts = [1.089701, 0.644028, 0.330027, 0.191695, 0.145157, 0.126463, 0.141158, 0.123946, 0.123072, 0.123810, 0.119789, 0.126160, 0.125554, 0.141566, 0.193552]
speed_ts = [1.00, 1.69, 3.30, 5.68, 7.51, 8.62, 7.72, 8.79, 8.85, 8.80, 9.10, 8.64, 8.68, 7.70, 5.63]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))
fig.suptitle('Hardware Saturation and Thread Scaling Limit (up to 1024 Threads)', fontsize=16, fontweight='bold')

# Left Plot: Execution Time
ax1.plot(x_pos, time_ts, 'mo-', label='Approach 2 (Local)', linewidth=2)
ax1.axvline(x=5, color='orange', linestyle='--', alpha=0.7, label='Hardware Limit (16 Threads)')
ax1.set_xticks(x_pos)
ax1.set_xticklabels(thread_labels, rotation=45)
ax1.set_xlabel('Number of Threads', fontsize=12)
ax1.set_ylabel('Execution Time (seconds)', fontsize=12)
ax1.set_title('Execution Time vs. Thread Oversubscription', fontsize=14)
ax1.grid(True, linestyle=':', alpha=0.6)
ax1.legend()

# Right Plot: Speedup
ax2.plot(x_pos, speed_ts, 'mo-', label='Approach 2 (Local)', linewidth=2)
ax2.axvline(x=5, color='orange', linestyle='--', alpha=0.7, label='Hardware Limit (16 Threads)')
ax2.axhline(y=1.0, color='gray', linestyle='-.', alpha=0.5, label='Baseline (1 Thread)')
ax2.set_xticks(x_pos)
ax2.set_xticklabels(thread_labels, rotation=45)
ax2.set_xlabel('Number of Threads', fontsize=12)
ax2.set_ylabel('Speedup Multiplier', fontsize=12)
ax2.set_title('Speedup Degradation at High Thread Counts', fontsize=14)
ax2.grid(True, linestyle=':', alpha=0.6)
ax2.legend()

plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.savefig('../results/thread_scaling_comparison.png', dpi=300)
print("Saved to ../results/thread_scaling_comparison.png")