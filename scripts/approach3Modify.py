import matplotlib.pyplot as plt

# Data from run3_mod output
threads = [1, 2, 4, 8, 16, 32, 64, 100]
thread_labels = [str(t) for t in threads]
x_pos = range(len(threads))

# Extreme Contention (k = 1)
time_ext = [1.920004, 3.834502, 4.295528, 5.073784, 4.053339, 4.015827, 3.990719, 3.994696]
speed_ext = [1.00, 0.50, 0.45, 0.38, 0.47, 0.48, 0.48, 0.48]

# Moderate Contention (k = 10000)
time_mod = [1.138881, 0.659385, 0.354425, 0.223435, 0.158411, 0.128723, 0.132371, 0.142954]
speed_mod = [1.00, 1.73, 3.21, 5.10, 7.19, 8.85, 8.60, 7.97]

# Local Accumulation (Near Ideal, k = 0)
time_loc = [1.149172, 0.668292, 0.375199, 0.212819, 0.177991, 0.132582, 0.132163, 0.139144]
speed_loc = [1.00, 1.72, 3.06, 5.40, 6.46, 8.67, 8.70, 8.26]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))
fig.suptitle('Trade-off Analysis: Synchronization Frequency', fontsize=16, fontweight='bold')

# Left Plot: Execution Time
ax1.plot(x_pos, time_ext, 'rs--', label='Extreme (k=1)', linewidth=2)
ax1.plot(x_pos, time_mod, 'g^-.', label='Moderate (k=10000)', linewidth=2)
ax1.plot(x_pos, time_loc, 'bo-', label='Local (k=0)', linewidth=2)
ax1.set_xticks(x_pos)
ax1.set_xticklabels(thread_labels)
ax1.set_xlabel('Number of Threads', fontsize=12)
ax1.set_ylabel('Execution Time (seconds)', fontsize=12)
ax1.set_title('Execution Time vs. Number of Threads', fontsize=14)
ax1.grid(True, linestyle=':', alpha=0.6)
ax1.legend()

# Right Plot: Speedup
ax2.plot(x_pos, speed_ext, 'rs--', label='Extreme (k=1)', linewidth=2)
ax2.plot(x_pos, speed_mod, 'g^-.', label='Moderate (k=10000)', linewidth=2)
ax2.plot(x_pos, speed_loc, 'bo-', label='Local (k=0)', linewidth=2)
ax2.axhline(y=1.0, color='gray', linestyle='-.', alpha=0.5, label='Baseline (1 Thread)')
ax2.set_xticks(x_pos)
ax2.set_xticklabels(thread_labels)
ax2.set_xlabel('Number of Threads', fontsize=12)
ax2.set_ylabel('Speedup Multiplier', fontsize=12)
ax2.set_title('Speedup vs. Number of Threads', fontsize=14)
ax2.grid(True, linestyle=':', alpha=0.6)
ax2.legend()

plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.savefig('../results/sync_frequency_tradeoff.png', dpi=300)
print("Saved to ../results/sync_frequency_tradeoff.png")