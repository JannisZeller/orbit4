"""Class assessing results of a orbit4-simulation. 

Version 0.2.0
"""

#%% Imports
# ------------------------------------------------------------------------------
import sys
from typing import Union
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation




#%% Main-Body
# ------------------------------------------------------------------------------
class System:
    ## Init
    def __init__(self,
        data: pd.DataFrame,
        colors: list[str]=None):
        """Initializes a System object which provides methods for assessing 
        results and construct visualizations.

        Parameters
        ----------
        data : pd.DataFrame
            DataFrame with the position-data of the bodies as a
            n_steps x (3*n_bodes) matrix.
        colors : list[str]
            Color for the bodies. Must be of length `int(df.shape[1] / 3)`.
        """

        ## Managing Data
        if isinstance(data, pd.DataFrame):
            data = data.values
        self.n_bodies = int(data.shape[1] / 3)
        self.data = self.reshape_data(data)
        self.n_timesteps = self.data.shape[1]

        ## Managing Colors
        if colors is None:
            self.colors = list(sns.color_palette("hls", self.n_bodies).as_hex())
        else:
            assert len(colors) == self.n_bodies, "Length of `colors` must be equal to the number of bodies, i. e. `int(df.shape[1] / 3)`."
            self.colors = colors


    ## Main Utility
    # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    ## Reshape data
    def reshape_data(self, data: np.ndarray, n_bodies: int=None) -> np.ndarray:
        """Reshapes the data from (n_steps, 3*n_bodies) to 
        (n_bodies, n_steps, 3).
        If n_bodies is None self.n_bodies is used.
        """
        if n_bodies == None:
            n_bodies = self.n_bodies
        data = np.stack([
            data[:, (3 * n):(3 * n+3)] 
            for n in range(n_bodies)])
        return data


    ## Plot the history of the system
    def plot_history(self, 
        n_steps: int=None,
        setmax:  bool=True, 
        mark_00: bool=True,
        figsize: tuple[int]=(10, 10)) -> plt.figure:
        """Plot the history / data in 3 dimensions. 

        Parameters
        ----------
        n_steps : int=None
            The first `n_steps` will be plotted. If None, all will be plotted.
        setmax : bool=True
            If True the axis-limits will be determined based on the radius of
            the first timestep, i. e. by using `_get_max_r()`.
        mark_00 : bool=True
            If True the coordinate system-center gets marked.
        figsize : tuple[int]=(10, 10)
            Size of the resulting figure.
        """
        ## Managing Data
        if n_steps == None:
            n_steps = self.n_timesteps

        ## Acutal Plotting
        fig = plt.figure(figsize=figsize)
        ax = fig.add_subplot(111, projection="3d")
        if mark_00 is True:
            ax.scatter(0, 0, color="k", s=100)
        if setmax == True:
            max_r = self._get_max_r()
            self._set_all_ax_lims(ax, max_r)
        for n in range(self.n_bodies):
            ax.plot(
                *(self.data[n, :, dim] for dim in range(3)), 
                color=self.colors[n])
            ax.scatter(
                *(self.data[n, -1, dim] for dim in range(3)), 
                color=self.colors[n])
        return fig


    ## Wrapper for general 3-D Plots
    def plot_custom(self, 
        data:    Union[pd.DataFrame, np.ndarray],
        max_r:   float=None, 
        mark_00: bool=True,
        colors:  list[str]=None,
        figsize: tuple[int]=(10, 10)) -> plt.figure:
        """Custom 3-D plots. 

        Parameters
        ----------
        data : Union[pd.DataFrame, np.ndarray]
            The custom data to be plotted. Shape can be (n_steps, 3*n_bodies) or
            (n_bodies, n_steps, 3). n_bodies gets inferred from here.
        max_r : float=None
            Axis-limits if not None
        mark_00 : bool=True
            If True the coordinate system-center gets marked.
        colors : list[str]=None
            Colors for the bodies.
        figsize : tuple[int]=(10, 10)
            Size of the resulting figure.
        """
        ## Managing Shape and Data
        if isinstance(data, pd.DataFrame):
            data = data.values
        if len(data.shape) == 3:
            n_bodies_detected = data.shape[0]
        elif len(data.shape) == 2:    
            n_bodies_detected = int(data.shape[1] / 3)
            data = self.reshape_data(data, n_bodies_detected)
        else:
            raise ValueError("n suitable dimensions of data. Must be (n_steps, 3*n_bodies) or (n_bodies, n_steps, 3)")
        
        ## Color settings
        if colors is None:
            colors = list(sns.color_palette("hls", n_bodies_detected).as_hex())

        ## Actual Plotting
        fig = plt.figure(figsize=figsize)
        ax = fig.add_subplot(111, projection="3d")
        if mark_00 is True:
            ax.scatter(0, 0, color="k", s=100)
        if max_r != None:
            self._set_all_ax_lims(ax, max_r)
        for n in range(n_bodies_detected):
            ax.plot(
                *(data[n, :, dim] for dim in range(3)), 
                color=self.colors[n])
            ax.scatter(
                *(data[n, -1, dim] for dim in range(3)), 
                color=self.colors[n])
        return fig
        

    ## Animation of the system
    def animate(self,
        ofname:     str="./test.gif",
        steps_per_frame: int=None,
        n_frames:   int=None,
        tempo:      float=50,
        last_step:  int=None,
        setmax:     bool=True,
        history:    bool=True,
        mark_00:    bool=True,
        ax_offset:  float=0.1,
        figsize:    tuple[int]=(15, 15)):

        ## Frames and Step-Settings
        assert (n_frames == None) != (steps_per_frame == None), "Either n_frames or steps_per_frame must / may be passed."
        if n_frames is None:
            n_frames = int(np.floor(self.n_timesteps / steps_per_frame))
        elif steps_per_frame is None:
            steps_per_frame = int(np.floor(self.n_timesteps / n_frames))

        print(f"Animating the System with {n_frames} frames, i. e. {steps_per_frame} steps per frame.")

        ## Managing and slicing Data
        if last_step == None:
            last_step = self.n_timesteps
        data_anim = self.data[:, :last_step:steps_per_frame, :]

        ## Initializing the figure
        fig = plt.figure(figsize=figsize)
        ax = fig.add_subplot(111, projection="3d")

        ## Animation Callbacks
        def configure_ax(ax):
            max_r = self._get_max_r()
            self._set_all_ax_lims(ax, max_r, ax_offset)
        def animate_step(step_idx):
            data_curr = data_anim[:, step_idx, :]
            ax.clear()
            if setmax == True:
                configure_ax(ax)
            for n in range(self.n_bodies):
                if history is True:
                    last_full_idx = step_idx*steps_per_frame                    
                    ax.plot(
                        *(self.data[n, :last_full_idx, dim] for dim in range(3)), 
                        color=self.colors[n])
                ax.scatter(
                    *(data_curr[n, dim] for dim in range(3)), 
                    color=self.colors[n])
                if mark_00 is True:
                    ax.scatter(0, 0, color="k", s=100)

        ## Acutal Animation
        ani = FuncAnimation(
            fig,
            animate_step,
            frames=np.arange(0, n_frames),
            interval=tempo,
            repeat=False,
            save_count=sys.maxsize,
        )
        ani.save(ofname)



    ## Helper Functions
    # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


    ## Maximum Distances
    def _get_max_r(self):
        """Gets the maximum radius (relative to (0, 0)) at timestep 0.
        """
        x = self._update(0)
        return np.max(np.linalg.norm(x, axis=1))


    ## Update state
    def _update(self, step_idx):
        """Update function, returning the current state at a given timestep.
        """
        state = self.data[:, step_idx, :]
        return state


    ## Setting Multiple Axis Limits
    def _set_all_ax_lims(self, ax: plt.Axes, max_r, offset_factor=0.05):
        offset = max_r * offset_factor
        min_ = -max_r - offset
        max_ =  max_r + offset
        ax.set_xlim(min_, max_)
        ax.set_ylim(min_, max_)
        ax.set_zlim(min_, max_)




#%% Unnecessairy main
# ------------------------------------------------------------------------------

if __name__ == "__main__":
    print("Hello World.")