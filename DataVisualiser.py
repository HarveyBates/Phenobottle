import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.animation as animation
import tkinter as tk
from tkinter.filedialog import askopenfile, askopenfilename, asksaveasfile
from tkinter.scrolledtext import ScrolledText
import webbrowser
import numpy as np

parameters = ['Optical Density', 'Temperature', 'pH', 'Chl Fluorescence']
colors = ['pink', 'yellow', 'red', 'purple', 'grey', 'orange', 'blue']

class MainWindow(tk.Frame):
	def __init__(self, master=None):
		super().__init__(master)
		self.master = master
		self.set_plot_font()
		self.menu_bar()

		x_values, y_values = self.get_data(100)

		for row, parameter in enumerate(parameters):
			self.create_plot()
			self.add_data('fill', parameter, x_values, y_values, 0, row, colors[row], False)
			x_values, y_values = self.get_data(100)
			self.add_data('fill', parameter, x_values, y_values, 0, row, colors[row-1], False)

		x_values, y_values = self.get_data(100)
		
		self.create_plot()
		for i in range(4):
			x_values, y_values = self.get_data(100)
			self.add_data('fill', parameter, x_values, y_values, 1, 0, colors[i+2], True)
		
		self.create_plot()
		offset = 0
		for i in range(2):
			x_values, y_values = self.get_data(100)
			y_values =np.sin(2 * np.pi * 5 * 100 * (x_values + offset) / 8000)
			self.add_data('Line', parameter, x_values, y_values, 1, 2, colors[i], True)
			offset += 500

	def menu_bar(self):
		self.menu = tk.Menu(self.master)
		root.config(menu=self.menu)

		# Create start menu items and assign their function
		self.fileMenu = tk.Menu(self.menu, tearoff=False)
		self.fileMenu.add_command(label="Load Config", command=self.open_file)
		self.fileMenu.add_command(label="Save Config As", command=self.save_config)
		self.fileMenu.add_separator()
		self.fileMenu.add_command(label="Exit", command=self.exit_program)
		self.helpMenu = tk.Menu(self.menu, tearoff=False)
		self.helpMenu.add_command(label="View Help", command=self.display_help)
		self.helpMenu.add_command(label="About", command=self.display_about)
		self.menu.add_cascade(label="File", menu=self.fileMenu)
		self.menu.add_cascade(label="Help", menu=self.helpMenu)

	def display_help(self):
		print("You need help")
		return

	def display_about(self):
		# Create about window
		self.aboutWindow = tk.Toplevel(self.master)
		self.aboutWindow.geometry('300x200')
		self.aboutWindow.resizable(False, False)
		self.aboutWindow.title("About the Phenobottle")

		#Create text in about window
		self.githubLink = tk.Label(self.aboutWindow, text="https://github.com/HarveyBates/Phenobottle",
		 fg="blue", cursor="hand2")
		self.aboutInfo = tk.Label(self.aboutWindow, text="The Phenobottle is an open-source photobioreactor. " 
			"To find out more please visit the devices GitHub page at: ".format(self.githubLink), 
			wraplength=250)
		self.emailInfo = tk.Label(self.aboutWindow, text="or Email:")
		self.emailContact = tk.Label(self.aboutWindow, text="harvey.bates@student.uts.edu.au",
		 fg="blue")
		
		# Assign text location
		self.aboutInfo.grid(sticky="nsew", row=0, padx=25, pady=10)
		self.githubLink.grid(sticky="nsew", row=1, pady=10)
		self.emailInfo.grid(sticky="nsew", row=2, pady=5)
		self.emailContact.grid(sticky="nsew", row=3, pady=5)
		self.emailContact.configure()

		# Link to GitHub website
		self.githubLink.bind("<Button-1>", lambda e: self.open_web_hyperlink("https://github.com/HarveyBates/Phenobottle"))

	def check_for_config(self):
		# Check to see if a config file exists
		global configuration_file
		configuration_file = "config.txt"
		if(path.exists(configuration_file)):
			self.update_config()
			self.master.title("Dashboard - {}".format("config.txt"))
		else:
			self.master.title("Dashboard - {}".format("Config File Not Found"))

	def update_config(self):
		# Update configuration settings
		config_file = open(configuration_file, "r")
		if (config_file != None):
			for i, config in enumerate(config_file):
				self.update_entry(self.entry[i], config.split("=")[1].strip())

	def open_file(self):
		# Open a configuration file
		config_file = askopenfile(mode='r', filetypes=[("Text Files", "*.txt")])
		if (config_file is not None):
			configuration_file = config_file.name.split("/")[-1]
			self.master.title("Dashboard - {}".format(configuration_file))
			for i, config in enumerate(config_file):
				self.update_entry(self.entry[i], config.split("=")[1].strip())


	def save_config(self):
		# Save current configuration of settings
		config_file = asksaveasfile(filetypes=[("Text Files", "*.txt")], mode='a')
		if(config_file != None):
			configuration_file = config_file.name.split("/")[-1]
			self.master.title("Dashboard - {}".format(configuration_file))
			with open(configuration_file, "w") as fwrite:
				for i, label in enumerate(labels):
					fwrite.write(label + "=%s" %self.entry[i].get())
			fwrite.close()

	def open_web_hyperlink(self, url):
		webbrowser.open_new(url) # Open web browser at required URL

	def exit_program(self):
		exit()

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

	def add_data(self, type, title, x_values, y_values, column, row, color, expand):

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
	
		if(type == 'fill'):	
			self.ax.fill_between(x_values, y_values.flatten(), color=color, alpha=0.3)
		else:
			self.ax.plot(x_values, y_values.flatten(), color=color, alpha=0.3, linewidth=3)
		self.fig.tight_layout()

	def get_data(self, size):
		y_data = np.random.rand(1, size).flatten()
		x_data = np.arange(0, size, 1)
		return x_data, y_data


root = tk.Tk()
root.wm_title("Phenobottle - Dashboard")
root.resizable(True, True)
root.geometry("1200x800")
# root.configure(bg="#111111")
root.grid_rowconfigure(0, weight=1)
root.grid_rowconfigure(1, weight=1)
root.grid_rowconfigure(2, weight=1)
root.grid_columnconfigure(0, weight=1)
root.grid_columnconfigure(1, weight=1)
app = MainWindow(master=root)
root.mainloop()


