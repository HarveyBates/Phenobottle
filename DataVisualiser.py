import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.animation as animation
import tkinter as tk
import numpy as np

parameters = ['Optical Density', 'Temperature', 'pH', 'Chl Fluorescence']
colors = ['blue', 'pink', 'orange', 'yellow', 'red', 'purple', 'grey']

class MainWindow(tk.Frame):
	def __init__(self, master=None):
		super().__init__(master)
		self.master = master
		self.set_plot_font()

		x_values, y_values = self.get_data(100)

		for row, parameter in enumerate(parameters):
			self.create_plot()
			self.add_data(parameter, x_values, y_values, 0, row, colors[row], False)
			x_values, y_values = self.get_data(100)
			self.add_data(parameter, x_values, y_values, 0, row, colors[row-1], False)

		x_values, y_values = self.get_data(100)
		self.create_plot()
		self.add_data(parameter, x_values, y_values, 1, 0, colors[5], True)

		x_values, y_values = self.get_data(100)
		self.create_plot()
		self.add_data(parameter, x_values, y_values, 1, 2, colors[4], True)


	def set_plot_font(self):
		plt.rc('font', family='sans-serif') 
		plt.rc('font', serif='Helvetica Neue') 
		plt.rc('text', usetex='false') 
		plt.rcParams.update({'font.size': 10})

	def create_plot(self):
		self.fig = plt.Figure(figsize=(5, 2), dpi=100)
		self.fig.patch.set_facecolor("#111111")
		self.ax = self.fig.add_subplot(1, 1, 1)
		self.canvas = FigureCanvasTkAgg(self.fig, root)

	def add_data(self, title, x_values, y_values, column, row, color, expand):

		self.ax.grid(True, which="major", axis='both', color='w', alpha=0.2)
		sides = ['top', 'bottom', 'left', 'right']
		for side in sides:
			self.ax.spines[side].set_color('#b3cccc')
		self.ax.tick_params(axis='x', colors='#b3cccc')
		self.ax.tick_params(axis='y', colors='#b3cccc')
		self.ax.xaxis.label.set_color('#b3cccc')
		self.ax.yaxis.label.set_color('#b3cccc')
		self.ax.set_xlabel('Time (min)')
		self.ax.set_ylabel(title)
		self.ax.set_title(title, color="white")
		self.ax.set_facecolor("#171717")
		
		if(expand == False):
			self.canvas.get_tk_widget().grid(column=column, row=row, sticky="nsew")
		else:
			self.canvas.get_tk_widget().grid(column=column, row=row, rowspan=2, sticky="nsew")
	
		self.ax.fill_between(x_values, y_values.flatten(), color=color, alpha=0.3)
		self.fig.tight_layout()

	def get_data(self, size):
		y_data = np.random.rand(1, size).flatten()
		x_data = np.arange(0, size, 1)
		return x_data, y_data

	

root = tk.Tk()
root.wm_title("Phenobottle - Dashboard")
root.resizable(True, True)
root.geometry("1200x800")
root.configure(bg="#111111")
root.grid_rowconfigure(0, weight=1)
root.grid_rowconfigure(1, weight=1)
root.grid_rowconfigure(2, weight=1)
root.grid_columnconfigure(0, weight=1)
root.grid_columnconfigure(1, weight=1)
app = MainWindow(root)
root.mainloop()


