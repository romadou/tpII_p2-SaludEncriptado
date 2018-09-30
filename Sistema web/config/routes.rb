Rails.application.routes.draw do

  scope module: 'api' do
    namespace :v1 do
      resources :patients, except: [:new, :edit]
      resources :sensors, except: [:new, :edit]
      resources :measures, except: [:new, :edit, :update]
    end
  end

  resources :patients
  resources :sensors, except: [:show]
  resources :measures, only: :destroy

  root "application#home"
end
