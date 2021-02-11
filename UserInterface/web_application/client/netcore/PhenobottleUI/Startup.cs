using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.HttpsPolicy;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using PhenobottleUI.Models;

namespace PhenobottleUI
{
    public class Startup
    {
        public Startup(IConfiguration configuration)
        {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        // This method gets called by the runtime. Use this method to add services to the container.
        public void ConfigureServices(IServiceCollection services)
        {
            // This is where to setup sign in functionality 
            services.AddControllersWithViews();
            services.AddEntityFrameworkNpgsql().AddDbContext<APIContext>(options =>
                options.UseNpgsql(Configuration.GetConnectionString("DefaultConnection")));
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }
            else
            {
                app.UseExceptionHandler("/Home/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }
            app.UseHttpsRedirection();
            app.UseStaticFiles();

            app.UseRouting();

            app.UseAuthorization();

            app.UseEndpoints(endpoints =>
            {
                endpoints.MapControllerRoute(
                    name: "Home",
                    pattern: "{controller=Home}/{action=Index}/{id?}");

                endpoints.MapControllerRoute(
                    name: "Dashboard",
                    pattern: "/{Dashboard}",
                    defaults: new
                    {
                        controller = "Dashboard",
                        action = "Index"
                    });              

                endpoints.MapControllerRoute(
                    name: "ControlPanel",
                    pattern: "/{ControlPanel}",
                    defaults: new { controller = "ControlPanel", 
						action = "Index" });

                endpoints.MapControllerRoute(
					name: "Experiment",
                    pattern: "/{Experiment}",
					defaults: new
					{
						controller = "Experiment",
						action = "Index"
					});

                endpoints.MapControllerRoute(
                    name: "Settings",
                    pattern: "/{Settings}",
                    defaults: new
                    {
                        controller = "Settings",
                        action = "Index"
                    });
            });
        }
    }
}
