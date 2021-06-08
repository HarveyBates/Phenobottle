from node:latest

# Copies files in this dir and install node if changes in package.json have occured
COPY ./package.json ./
RUN npm install
COPY ./ ./

CMD npm run dev
